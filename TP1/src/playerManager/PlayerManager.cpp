#include "../headers/playerManager/PlayerManager.h"

#include "CommandManager.h"
#include "TeamManager.h"
#include "MatchManager.h"
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include "tools/ProcessSpawner.h"


PlayerManager::PlayerManager(unsigned maxPlayersVillage, unsigned maxMatchesPerPlayer){
	if(minPlayersToBeginGame > maxPlayersVillage){
		log(PLAYER_MANAGER_NAME + " : **Error** cantidad maxima de jugadores en el predio es menor a ",minPlayersToBeginGame,ERROR);
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
	this->amountTeamsFormed = 0;
	this->amountPlayersToWait = 0;
}

PlayerManager::~PlayerManager(){
	this->channelToRead->cerrar();
	this->channelToWrite->cerrar();
	this->channelToWriteResult->cerrar();
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


void PlayerManager::executeTeamManager(){
    log("INICIO DEL TEAM_MANAGER",INFORMATION);
    TeamManager *teamManager = new TeamManager();
    teamManager->execute();
    delete teamManager;
    log("FIN DEL TEAM_MANAGER",INFORMATION);
}

void PlayerManager::executeMatchManager(){
    log("INICIO DEL MATCH_MANAGER",INFORMATION);
    MatchManager *matchManager = new MatchManager();
    matchManager->execute();
    delete matchManager;
    log("FIN DEL MATCH_MANAGER",INFORMATION);
}



void PlayerManager::execute(){

	pid_t pid;
	ProcessSpawner * processSpawner = new ProcessSpawner();
	pid = processSpawner->spawnProcess(PlayerManager::executeCommandManager);


	std::vector<functiontype> *functions = new std::vector<functiontype>();
    functions->push_back(PlayerManager::executeTeamManager);
    functions->push_back(PlayerManager::executeMatchManager);
	processSpawner->spawnProcesses(functions);
	delete functions;

	this->channelToRead->abrir();
	this->channelToWrite->abrir();
	this->channelToWriteResult->abrir();
	log(PLAYER_MANAGER_NAME + " :  Se abrio FIFO de lectura " + FIFO_READ_COMMAND_OF_COMMANDMANAGER,INFORMATION);
	log(PLAYER_MANAGER_NAME + " :  Se abrio FIFO de escritura " + FIFO_WRITE_PLAYER_TO_TEAMMANAGER,INFORMATION);
	log(PLAYER_MANAGER_NAME + " :  Se abrio FIFO de escritura " + FIFO_READ_RESULT_TO_RESULTMANAGER,INFORMATION);
	

	struct messagePlayer* message;
	std::cout<<"DEBEN INGRESAR AL MENOS "<<this->minPlayersToBeginGame<<" JUGADORES PARA INICIAR EL JUEGO"<<std::endl;
	while(!this->finalizedProcess){
		message = this->readFifoPlayerManager();
		this->parseMessage(message);
		if(this->beginGame && this->amountPlayersToWait == 0){
			this->evaluateEndGame();
			this->writeFifoTeamManager();
			this->writeFlagToFormTeam();
		}
		delete message;		
	}

	log(PLAYER_MANAGER_NAME + " : El proceso PlayerManager finaliza correctamente ",INFORMATION);
	flushLog();

	this->sendSingnalToProcess(pid);
	this->loggearPlayers();
	

	pid = wait(NULL);
	if(pid != -1){
		log(PLAYER_MANAGER_NAME + " : Proceso finalizado, su pid es ", pid, INFORMATION);
	}else{
		log(PLAYER_MANAGER_NAME + " : Error en la finalizacion del proceso ", INFORMATION);
	}
	flushLog();

	processSpawner->waitChilds();

	delete processSpawner;
}


void PlayerManager::sendSingnalToProcess(pid_t pid){
	int ss = kill(pid,SIGINT);
	if (ss == 0){
		log(PLAYER_MANAGER_NAME + " :  señal de kill enviada a CommandManager correctamente ",INFORMATION);
	}else if(ss == -1){
		log(PLAYER_MANAGER_NAME + " :  **Error** al enviar señal de kill al proceso", pid, ERROR);
	}
}



/**
 * Se evalua si todos los jugadores completaron la cantidad de juegos
 **/
void PlayerManager::evaluateEndGame(){
	std::vector<PlayerPM*>::iterator it;
	PlayerPM *player;
	bool isVillageFull	= this->playersToGame->size() == this->maxPlayersVillage;

	//primero se evalua si hay jugadores en espera
	if(isVillageFull == false){
		//it = this->playersToWait->begin();
		while(!this->playersToWait->empty() && isVillageFull == false){
			player = this->playersToWait->back();
			this->playersToGame->push_back(player);
			this->playersToWait->pop_back();
			log(PLAYER_MANAGER_NAME + " :  jugador en espera entra al predio porque el predio no esta lleno, jugador con id: ",player->getId(), INFORMATION);
			flushLog();									
			isVillageFull = this->playersToGame->size() == this->maxPlayersVillage;
		}
	}

	// se evalua si no hay jugadores, tambien se determina en updateAmountPlayersToWait
	// updateAmountPlayersToWait evalua si no se pueden formar mas equipos con jugadores 
	//evalua si no hay jugadores, complementado al anterior
	if(this->playersToGame->empty()){
		log("SE CUMPLE CONDICIÓN FINALIZACIÓN DEL JUEGO",INFORMATION);
		//MANDAR UNA SIGNAL A COMMANDMANAGER
		this->finalizedProcess = true;
		this->beginGame = false;
		struct messagePlayer message;
		message.status = CommandType::killType; 
		this->writeMessagePlayer(&message);
		this->writeEndGameToResultManager();
	}
}




void PlayerManager::loggearPlayers(){
	
	std::vector<PlayerPM*>::iterator it;
	std::string players = "";

	for(it = this->playersToGame->begin();it != this->playersToGame->end(); it++){
		players += (*it)->logMemberVariables() + ", ";
	}
	log(PLAYER_MANAGER_NAME + " : Fin del proceso, jugadores que quedaron el en predio: "+ players,INFORMATION);
	
	players = "";
	for(it = this->playersToWait->begin();it != this->playersToWait->end(); it++){
		players += (*it)->logMemberVariables() + ", ";
	}
	log(PLAYER_MANAGER_NAME + " : Fin del proceso, jugadores que quedaron en espera: " + players,INFORMATION);
	
}


void PlayerManager::parseMessage(struct messagePlayer* message){

	switch (message->status){
		
		case CommandType::killType :
			this->finalizedProcess = true;
			this->writeMessagePlayer(message);
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

		//mensaje que informa la cantidad de equipos que se han formado
		case CommandType::amountTeams :
			this->updateAmountPlayersToWait(message->idPlayer);
			break;
	}
}


void PlayerManager::updateAmountPlayersToWait(int amountTeamsFormed){

	unsigned previusValue = this->amountPlayersToWait;

	this->amountTeamsFormed += amountTeamsFormed;
	this->amountPlayersToWait = (this->amountTeamsFormed/2);
	this->amountPlayersToWait *= 4;
	this->amountTeamsFormed %= 2;

	//condicion de finalizacion porque no se formaron mas de 2 equipos
	if(this->amountPlayersToWait == 0){
		log(PLAYER_MANAGER_NAME + " : SE CUMPLE CONDICIÓN FINALIZACIÓN DEL JUEGO",INFORMATION);
		//MANDAR UNA SIGNAL A COMMANDMANAGER
		this->finalizedProcess = true;
		this->beginGame = false;
		struct messagePlayer message;
		message.status = CommandType::killType; 
		this->writeMessagePlayer(&message);
		this->writeEndGameToResultManager();
	}


	// si llego algun jugador antes de que teamManager mande la cantidad de quipos creados
	if(previusValue != this->maxPlayersVillage){
		log(PLAYER_MANAGER_NAME + " : llego algun jugador de un match antes de que TeamManager informe la cant de equipos formados",INFORMATION);
		previusValue = this->maxPlayersVillage - previusValue;
		this->amountPlayersToWait -= previusValue;
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
			log(PLAYER_MANAGER_NAME + " :  jugador entra al predio luego de estar en espera por un comando agregar, jugador con id: ",playerToGame->getId(), INFORMATION);						
		}else{
			PlayerPM* player =  new PlayerPM(this->generateId());
			this->playersToGame->push_back(player);
			log(PLAYER_MANAGER_NAME + " :  jugador entra al predio por un comando agregar, jugador con id: ",player->getId(), INFORMATION);						
			
		}

	}else{
		PlayerPM* playerToWait =  new PlayerPM(this->generateId());
		this->playersToWait->push_back(playerToWait);
		log(PLAYER_MANAGER_NAME + " :  jugador entra por comando agregar en modo espera porque el predio esta lleno , jugador con id: ",playerToWait->getId(), INFORMATION);						
		
	}

	//evaluamos si puede comenzar el juego
	//solo se ingresa una vez
	if(!this->beginGame && this->playersToGame->size() >= minPlayersToBeginGame ){	
		this->beginGame = true;
		std::cout<<"COMIENZA EL JUEGO"<<std::endl;
		log(PLAYER_MANAGER_NAME + " :  Se completa la cantidad minima de jugadores para comenzar el juego",INFORMATION);
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
			buff->status = CommandType::waitToTeam;
			this->writeMessagePlayer(buff);
			log(PLAYER_MANAGER_NAME + " : Se ha enviado para formar equipo al jugador con id: ",buff->idPlayer,INFORMATION);
			delete buff;
		}
	}
	//cambiamos a la cantidad de jugadores a esperar a la cant max del predio
	//hasta que llegue una actualización del TeamManager
	this->amountPlayersToWait = this->maxPlayersVillage;

}

void PlayerManager::writeFlagToFormTeam(){

	struct messagePlayer *buff = new messagePlayer;
	memset(buff,0,sizeof(messagePlayer));
	buff->status = CommandType::readyToTeam;
	this->writeMessagePlayer(buff);
	log(PLAYER_MANAGER_NAME + " : Se ha enviado el Flag a TeamManager para comenzar a formar equipos",INFORMATION);
	delete buff;

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
		log(PLAYER_MANAGER_NAME + " : **ERROR***  no se pudo mandar finalización de juego a ResultManager ", __FILE__, __LINE__, ERROR);
		exit(1);
	}else if (result != sizeof(messageResult)){
		log(PLAYER_MANAGER_NAME + " : **ERROR***  no se pudo mandar se mando correctamente finalización de juego a ResultManager", __FILE__, __LINE__, ERROR);
		exit(1);
	}else{
		log(PLAYER_MANAGER_NAME + " :  se mando correctamente finalización de juego a ResultManager ", INFORMATION);
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
	log(PLAYER_MANAGER_NAME + " :  llega mensaje de cancelacion de partido para jugador: ",message->idPlayer, INFORMATION);

		std::vector<PlayerPM*>::iterator it = this->playersToGame->begin();
		bool found = false;
		while(it != this->playersToGame->end() && !found){
			if((*it)->getId() == message->idPlayer){
				(*it)->endGame();//cambiamos el estado a libre
				this->amountPlayersToWait--; //disminuimos la cantidad de jugadores a esperar
				found = true;
				if(this->removePlayer > 0){	//si es necesario sacar a un jugador
					log(PLAYER_MANAGER_NAME + " :  jugador sale del predio por comando, jugador con id: ",message->idPlayer, INFORMATION);
					this->playersToWait->push_back((*it));
					this->playersToGame->erase(it);
					this->removePlayer--;
				}
			}
			it++;
		}
		if(!found){
			log(PLAYER_MANAGER_NAME + " :  jugador no encontrado en el predio para notificar partido cancelado su estado id jugador: ",message->idPlayer, ERROR);
			exit(1);
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

	while(it != this->playersToGame->end() && !found){
		player = (*it);
		if(player->getId() == idPlayer){
			found = true;
			player->addGamePlayed();
			log(PLAYER_MANAGER_NAME + ": Aumentado la cantidad de partidos del jugador con id: ",player->getId(),INFORMATION);
			player->endGame();	//el jugador esta libre para volver a jugar
			this->amountPlayersToWait--; //disminuimos la cantidad de jugadores a esperar
			completedGames = this->evaluteGamesCompletedPlayer(player);

			if(completedGames){
				//si completo el juego lo elimino definitivamente.
				this->playersToGame->erase(it);
				delete player;
			}else if(this->removePlayer > 0){
				//si NO completo el juego solo lo saco del predio
				log(PLAYER_MANAGER_NAME + " :  jugador sale del predio por comando, jugador con id: ",idPlayer, INFORMATION);				
				this->playersToWait->push_back(player);
				this->playersToGame->erase(it);
				this->removePlayer--; 
			}
		}
		it++;
	}

	if(!found){
		log(PLAYER_MANAGER_NAME + " :  jugador no encontrado en el predio para actualizar su estado id jugador: ",idPlayer, ERROR);
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
		log(PLAYER_MANAGER_NAME + " :  Jugador ha completado los partidos permitidos, jugador con id ",player->getId(),INFORMATION);
	}else if(player->getGamesPlayed() > this->maxMatchesPerPlayer){
		log(PLAYER_MANAGER_NAME + " :  Jugador ha jugado mas partidos de los permitidos, jugador con id ",player->getId(),ERROR);
		exit (1); 
	}
	return completedGames;
}
