#include "../headers/playerManager/PlayerManager.h"

PlayerManager::PlayerManager(int maxPlayersVillage, int maxMatchesPerPlayer){
	this->channelToRead = new FifoLectura(FIFO_READ_COMMAND_OF_COMMANDMANAGER);
	this->channelToWrite = new FifoEscritura(FIFO_WRITE_PLAYER_TO_TEAMMANAGER);	
	this->playersToGame = new std::vector<PlayerPM*>();
	this->playersToWait = new std::vector<PlayerPM*>();
	this->idPlayer = 0;
	this->maxPlayersVillage = maxPlayersVillage;
	this->maxMatchesPerPlayer = maxMatchesPerPlayer;
	this->finalizedProcess = false;
	this->removePlayer = false;
}

PlayerManager::~PlayerManager(){
	this->channelToRead->cerrar();
	this->channelToWrite->cerrar();

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


void PlayerManager::execute(){
	this->channelToRead->abrir();
	this->channelToWrite->abrir();
	struct messagePlayer* message;
	while(!this->finalizedProcess){
		message = this->readFifoPlayerManager();
		this->parseMessage(message);
		this->writeFifoTeamManager();
		delete message;		
	}
	log(PLAYER_MANAGER_NAME + " : El proceso PlayerManager finaliza correctamente ",INFORMATION);
	log(PLAYER_MANAGER_NAME + " : la jugadores en predio ",this->playersToGame->size(),INFORMATION);
	log(PLAYER_MANAGER_NAME + " : la jugadores en espera ",this->playersToWait->size(),INFORMATION);
}


//falta terminar parser
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

		//mesage de Court notificando que el jugador no completo el partido	
		//notifica a TeamManager que el ultimo equipo del jugador no jugó
		case CommandType::gameCanceled :
			//this->writeMessagePlayer(message);
			this->notifyGameCanceled(message);
			break;

		//mesage de Court notificando que el jugador completo el partido
		//incrementa la cantidad de partidos jugados del jugador
		case CommandType::gameCompleted :
			this->updateMatchesPlayer(message->idPlayer);
			break;
	}
}


/*agrega un jugador, si el predio tiene capacidad se agrega al mismo
*si el predio no tiene capacidad se pone en espera
*/
void PlayerManager::addPlayerToGame(){

	//si en el predio no esta lleno
	if ((int)this->playersToGame->size() < this->maxPlayersVillage){

		//si hay jugadores en espera se agrega uno de ellos al predio, sino se crea 1 nuevo
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


/*
* escribe un jugador al teamManager si este se encuentra libre
*/
void PlayerManager::writeFifoTeamManager(){
	std::vector<PlayerPM*>::iterator it;
	for(it = this->playersToGame->begin();it != this->playersToGame->end();it++){
		if((*it)->isFree()){
			//envia un jugador al TeamManager
			(*it)->playGame(); 	//el jugador esta ocupado 
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
			log(PLAYER_MANAGER_NAME + " : No se pudo realizar la escritura en el fifo ", __FILE__, ERROR);
		}else if (result != sizeof(messagePlayer)){
			log(PLAYER_MANAGER_NAME + " : Se ha escrito una cantidad erronea de bytes en el fifo ", __FILE__, __LINE__, ERROR);
		}

}



/**
 * saca a un player libre del predio, si no hay jugador libre 
 * setea una variable para que se el proxmo libre salga del predio
**/
void PlayerManager::removePlayerToGame(){
	std::vector<PlayerPM*>::iterator it;
	bool found = false;

	for(it = this->playersToGame->begin();it != this->playersToGame->end();it++){
		if((*it)->isFree()){
			this->playersToWait->push_back((*it));
			this->playersToGame->erase(it);
			found = true;
		}
	}

	/*
	La busqueda es innecesaria porque nunca va a encontrar uno libre,
	 dado que cuando llega uno se envia inmediatamente...
	*/

	if (!found){
		//remover el primero que llegue después de jugar
		this->removePlayer = true;
	}
}


/**
 * para notificar a teamManager que no se completo el partido del jugador
 * evalua si es necesario sacar al jugador del predio, debido a un comando
 * cambia el estado del jugador a libre
 * */
void PlayerManager::notifyGameCanceled(struct messagePlayer* message){
	if(this->removePlayer){
		bool found = false;
		std::vector<PlayerPM*>::iterator it = this->playersToGame->begin();

		while(it != this->playersToGame->end() || !found){
			if((*it)->getId() == message->idPlayer){
				(*it)->endGame();//cambiamos el estado a libre
				this->playersToWait->push_back((*it));
				this->playersToGame->erase(it);
				found = true;
				this->removePlayer = false;
			}
			it++;
		}

		if(!found){
			log("jugador no encontrado en el predio para actualizar su estado id jugador: ",message->idPlayer, ERROR);
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
			}else if(this->removePlayer ){
				//si NO completo el juego solo lo saco del predio
				this->playersToWait->push_back(player);
				this->playersToGame->erase(it);
				this->removePlayer = false; 
			}

			/*if(this->removePlayer || completedGames){
				this->playersToWait->push_back(player);
				this->playersToGame->erase(it);
				//si el jugador completo el juego no cuenta como removido por comando
				this->removePlayer = completedGames; 
			}*/
		}
		it++;
	}

	if(!found){
		log("jugador no encontrado en el predio para actualizar su estado id jugador: ",idPlayer, ERROR);
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
		log("Jugador ha completado los partidos permitidos, jugador con id ",player->getId(),INFORMATION);
	}else if(player->getGamesPlayed() > this->maxMatchesPerPlayer){
		log("Jugador ha jugado mas partidos de los permitidos, jugador con id ",player->getId(),ERROR);
		exit (1);
	}
	return completedGames;
}
