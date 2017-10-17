#include "../headers/playerManager/PlayerManager.h"
#include "CommandManager.h"
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

PlayerManager::PlayerManager(unsigned maxPlayersVillage, unsigned maxMatchesPerPlayer){
	if(minPlayersToBeginGame < maxPlayersVillage){
		log("PlayerManager: cantidad maxima de jugadores en el predio es menor a ",maxPlayersVillage,INFORMATION);
		exit(1); 
	}
	this->channelToRead = new FifoLectura(FIFO_READ_COMMAND_OF_COMMANDMANAGER);
	this->channelToWrite = new FifoEscritura(FIFO_WRITE_PLAYER_TO_TEAMMANAGER);
	this->channelToWriteResult = new FifoEscritura(FIFO_READ_RESULT_TO_RESULTMANAGER);	

	this->playersToGame = new std::vector<PlayerPM*>();
	this->playersToWait = new std::vector<PlayerPM*>();
	this->idPlayer = 0;
	this->maxPlayersVillage = maxPlayersVillage;
	this->maxMatchesPerPlayer = maxMatchesPerPlayer;
	this->finalizedProcess = false;
	this->beginGame = false;
	this->removePlayer = 0;
}

PlayerManager::~PlayerManager(){
	this->channelToRead->cerrar();
	this->channelToWrite->cerrar();
	this->channelToWriteResult->cerrar();
	this->channelToRead->eliminar();
	this->channelToWrite->eliminar();
	this->channelToWriteResult->eliminar();

	this->channelToWriteResult->abrir();
	delete this->channelToRead;
	delete this->channelToWrite;

	std::vector<PlayerPM*>::iterator it;
	for(it = this->playersToGame->begin();it != this->playersToGame->end();it++){
		delete (*it);
	}
	for(it = this->playersToWait->begin();it != this->playersToWait->end();it++){
		delete (*it);
	}

	delete this->playersToGame;
	delete this->playersToWait;
}


int PlayerManager::generateId(){
	return this->idPlayer++;
}

void PlayerManager::executeCommandManager(){
    log("INICIO DEL COMMAND_MANAGER",INFORMATION);
    CommandManager *commandManager = new CommandManager();
    commandManager->execute();
    delete commandManager;
    log("FIN DEL COMMAND_MANAGER",INFORMATION);
}


void PlayerManager::execute(){

	pid_t pid = fork();
	bool isChild = pid == 0;
	if( isChild ){
		this->executeCommandManager();
		exit ( 0 );
	}else{
		this->channelToRead->abrir();
		this->channelToWrite->abrir();
		this->channelToWriteResult->abrir();

		struct messagePlayer* message;
		std::cout<<"DEBEN INGRESAR AL MENOS "<<this->minPlayersToBeginGame<<" JUGADORES PARA INICIAR EL JUEGO"<<std::endl;
		while(!this->finalizedProcess){
			message = this->readFifoPlayerManager();
			this->parseMessage(message);
			if(this->beginGame){
				this->writeFifoTeamManager();
				this->evaluateEndGame();
			}
			delete message;		
		}
		log(PLAYER_MANAGER_NAME + " : El proceso PlayerManager finaliza correctamente ",INFORMATION);
		flushLog();

		int ss = kill(pid,SIGINT);
		if (ss == 0){
			log("PlayerManager: señal de kill enviada a CommandManager correctamente ",INFORMATION);
		}else if(ss == -1){
			log("PlayerManager: error al enviar señal de kill a CommandManager",ERROR);
			exit(1);
		}

		this->loggearPlayers();
	}

	pid = wait(NULL);
	if(pid != -1){
		log("Proceso finalizado, su pid es ", pid, INFORMATION);
	}else{
		log("Error en la finalizacion del proceso ", INFORMATION);
	}
	flushLog();

}

/**
 * Se evalua si todos los jugadores completaron la cantidad de juegos
 **/
void PlayerManager::evaluateEndGame(){
	std::vector<PlayerPM*>::iterator it;
	//unsigned maxPossibleMatches = this->maxMatchesPerPlayer -1;
	bool isVillageFull	= this->playersToGame->size() == this->maxPlayersVillage;

	//primero se evalua si hay jugadores en espera
	if(isVillageFull == false){
		it = this->playersToWait->begin();
		while(!this->playersToWait->empty() && isVillageFull == false){
			this->playersToGame->push_back((*it));
			this->playersToWait->erase(it);
			isVillageFull = this->playersToGame->size() == this->maxPlayersVillage;
			it++;
		}
	}

	if(this->playersToGame->size() <= 3){
		std::cout<<"FINALIZACIÓN DEL JUEGO"<<std::endl;
		//MANDAR UNA SIGNAL A COMMANDMANAGERqq
		this->finalizedProcess = true;
		this->beginGame = false;
		struct messagePlayer message;
		message.status = CommandType::killType; 
		this->writeMessagePlayer(&message);
		this->writeEndGameToResultManager();
	}
}




void PlayerManager::loggearPlayers(){
	log(PLAYER_MANAGER_NAME + " : la jugadores en predio ",this->playersToGame->size(),INFORMATION);
	log(PLAYER_MANAGER_NAME + " : la jugadores en espera ",this->playersToWait->size(),INFORMATION);
	std::vector<PlayerPM*>::iterator it;

	for(it = this->playersToGame->begin();it != this->playersToGame->end(); it++){
		log((*it)->logMemberVariables(),INFORMATION);
	}

	for(it = this->playersToWait->begin();it != this->playersToWait->end(); it++){
		log((*it)->logMemberVariables(),INFORMATION);
	}

}


void PlayerManager::parseMessage(struct messagePlayer* message){

	switch (message->status){
		
		case CommandType::killType :
			this->finalizedProcess = true;
			this->writeMessagePlayer(message);
			//sleep(10);
			break;

		case CommandType::addType :
			this->addPlayerToGame();
			break;

		case CommandType::removeType :
			this->removePlayerToGame();
			break;

		//mensaje de Court notificando que el jugador no completo el partido	
		//notifica a TeamManager que el ultimo equipo del jugador no jugó
		case CommandType::gameCanceled :
			//this->writeMessagePlayer(message);
			this->notifyGameCanceled(message);
			break;

		//mensaje de Court notificando que el jugador completo el partido
		//incrementa la cantidad de partidos jugados del jugador
		case CommandType::gameCompleted :
			this->updateMatchesPlayer(message->idPlayer);
			break;
	}
}


/**
 * agrega un jugador, si el predio tiene capacidad se agrega al mismo
 *si el predio no tiene capacidad se pone en espera
**/
void PlayerManager::addPlayerToGame(){

	//si en el predio no esta lleno
	if (this->playersToGame->size() < this->maxPlayersVillage){
		//si hay jugadores en espera se agrega uno de ellos al predio
		if(!this->playersToWait->empty()){
			PlayerPM* playerToGame = this->playersToWait->back();
			this->playersToWait->pop_back();
			this->playersToGame->push_back(playerToGame);
		}else{
			PlayerPM* player =  new PlayerPM(this->generateId());
			this->playersToGame->push_back(player);
		}

	}else{
		PlayerPM* playerToWait =  new PlayerPM(this->generateId());
		this->playersToWait->push_back(playerToWait);
	}

	//evaluamos si puede comenzar el juego
	if(this->playersToGame->size() >= minPlayersToBeginGame && !this->beginGame){	
		this->beginGame = true;
		std::cout<<"COMIENZA EL JUEGO"<<std::endl;
		log("PlayerManager: Se completa la cantidad minima de jugadores para comenzar el juego",INFORMATION);
	}

}


struct messagePlayer* PlayerManager::readFifoPlayerManager(){
	struct messagePlayer *buff = new messagePlayer;
	memset(buff,0,sizeof(messagePlayer));
	int result = this->channelToRead->leer(buff,sizeof(messagePlayer));

	if(result == -1){
		log(PLAYER_MANAGER_NAME + " : No se pudo realizar la lectura del fifo ",__FILE__, __LINE__, ERROR);
	}else if (result != sizeof(messagePlayer)){
		log(PLAYER_MANAGER_NAME + " : Se ha leido una cantidad erronea de bytes del fifo ", __FILE__, __LINE__, ERROR);
	}
	
	return buff;
}


void PlayerManager::writeFifoTeamManager(){
	std::vector<PlayerPM*>::iterator it;
	for(it = this->playersToGame->begin();it != this->playersToGame->end();it++){
		if((*it)->isFree()){
			//envia un jugador al TeamManager
			(*it)->playGame(); 	//el jugador se cambia a ocupado 
			struct messagePlayer *buff = new messagePlayer;
			memset(buff,0,sizeof(messagePlayer));
			buff->idPlayer = (*it)->getId();
			this->writeMessagePlayer(buff);
			delete buff;
		}
	}
}


//escribe al fifo de teamManager
void PlayerManager::writeMessagePlayer(struct messagePlayer* message){

	int result = this->channelToWrite->escribir(message,sizeof(messagePlayer));
	if(result == -1){
		log(PLAYER_MANAGER_NAME + " : No se pudo realizar la escritura en el fifoTeam ", __FILE__, ERROR);
		exit(1);
	}else if (result != sizeof(messagePlayer)){
		log(PLAYER_MANAGER_NAME + " : Se ha escrito una cantidad erronea de bytes en el fifoTeam ", __FILE__, __LINE__, ERROR);
		exit(1);
	}

}


//escribe al fifo de ResultManager
void PlayerManager::writeEndGameToResultManager(){

	struct messageResult message;
	message.operation = ResultCommands::EXIT;
	
	int result = this->channelToWriteResult->escribir(&message,sizeof(messageResult));
	if(result == -1){
		log(PLAYER_MANAGER_NAME + " : No se pudo realizar la escritura en el fifoResult ", __FILE__, __LINE__, ERROR);
		exit(1);
	}else if (result != sizeof(messageResult)){
		log(PLAYER_MANAGER_NAME + " : Se ha escrito una cantidad erronea de bytes en el fifoResult ", __FILE__, __LINE__, ERROR);
		exit(1);
	}
	
}


void PlayerManager::removePlayerToGame(){
	this->removePlayer++;
}



/**
 * para notificar a teamManager que no se completo el partido del jugador
 * evalua si es necesario sacar al jugador del predio, debido a un comando
 * cambia el estado del jugador a libre
 * */
void PlayerManager::notifyGameCanceled(struct messagePlayer* message){
	//si es necesario sacar a un jugador
	if(this->removePlayer > 0){
		bool found = false;
		std::vector<PlayerPM*>::iterator it = this->playersToGame->begin();

		while(it != this->playersToGame->end() || !found){
			if((*it)->getId() == message->idPlayer){
				(*it)->endGame();//cambiamos el estado a libre
				this->playersToWait->push_back((*it));
				this->playersToGame->erase(it);
				found = true;
				this->removePlayer--;
			}
			it++;
		}

		if(!found){
			log("PlayerManager: jugador no encontrado en el predio para notificar partido cancelado su estado id jugador: ",message->idPlayer, ERROR);
			exit(1);
		}
	}
	this->writeMessagePlayer(message);
}



/**
 * aumenta la cantidad de partidos jugados de un player
 * evalua si es necesario sacar al jugador si completo todos sus partidos
 * evalua si es necesario sacar al jugador del predio, debido a un comando
 * cambia el estado del jugador a libre
 * **/
void PlayerManager::updateMatchesPlayer(int idPlayer){
	std::vector<PlayerPM*>::iterator it = this->playersToGame->begin();
	bool found = false;
	bool completedGames = false;
	PlayerPM* player;

	while(it != this->playersToGame->end() || !found){
		player = (*it);
		if(player->getId() == idPlayer){
			found = true;
			player->addGamePlayed();
			player->endGame();	//el jugador esta libre para volver a jugar
			completedGames = this->evaluteGamesCompletedPlayer(player);

			if(completedGames){
				//si completo el juego lo elimino definitivamente.
				this->playersToGame->erase(it);
				delete player;
			}else if(this->removePlayer > 0){
				//si NO completo el juego solo lo saco del predio
				this->playersToWait->push_back(player);
				this->playersToGame->erase(it);
				this->removePlayer--; 
			}
		}
		it++;
	}

	if(!found){
		log("PlayerManager: jugador no encontrado en el predio para actualizar su estado id jugador: ",idPlayer, ERROR);
		exit(1);
	}
}


/**
 * evalua si un jugador ha completado la maxima cantidad de partidos
 * el jugador se encuentra en el predio osea en playersToGame
 * si los ha completado se saca del predio definitivamente
 **/ 

bool PlayerManager::evaluteGamesCompletedPlayer(PlayerPM *player){

	bool completedGames = player->getGamesPlayed() == this->maxMatchesPerPlayer;

	if(completedGames){
		//player->gameOver(); //completo el juego
		log("PlayerManager: Jugador ha completado los partidos permitidos, jugador con id ",player->getId(),INFORMATION);
	}else if(player->getGamesPlayed() > this->maxMatchesPerPlayer){
		log("PlayerManager: Jugador ha jugado mas partidos de los permitidos, jugador con id ",player->getId(),ERROR);
		exit (1); 
	}
	return completedGames;
}
