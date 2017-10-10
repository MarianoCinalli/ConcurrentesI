#include "../headers/playerManager/PlayerManager.h"

PlayerManager::PlayerManager(int maxPlayersVillage, int maxMatchesPerPlayer){
	this->channelToRead = new FifoLectura(FIFO_READ_PLAYER_OF_COMMAND);
	this->channelToWrite = new FifoEscritura(FIFO_WRITE_PLAYER_TO_TEAM);	
	this->playersToGame = new std::vector<Player*>();
	this->playersToWait = new std::vector<Player*>();
	this->idPlayer = 0;
	this->maxPlayersVillage = maxPlayersVillage;
	this->maxMatchesPerPlayer = maxMatchesPerPlayer;
	this->finalizedProcess = false;
}

PlayerManager::~PlayerManager(){
	this->channelToRead->cerrar();
	this->channelToWrite->cerrar();

	delete this->channelToRead;
	delete this->channelToWrite;

	std::vector<Player*>::iterator it;

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
	}
	log("El proceso PlayerManager finaliza correctamente ",INFORMATION);
	log("la jugadores en predio ",this->playersToGame->size(),INFORMATION);
	log("la jugadores en espera ",this->playersToWait->size(),INFORMATION);
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
			this->writeMessagePlayer(message);
			break;

		//mesage de Court notificando que el jugador completo el partido
		//incrementa la cantidad de partidos jugados del jugador
		case CommandType::gameCompleted :
			this->updateMatchesPlayer(message->idPlayer);
			break;
	}
	
	delete message;
}


/*agrega un jugador, si el predio tiene capacidad se agrega al mismo
*si el predio no tiene capacidad se pone en espera
*/
void PlayerManager::addPlayerToGame(){

	//si en el predio no esta lleno
	if ((int)this->playersToGame->size() <= this->maxPlayersVillage){

		//si hay jugadores en espera se agrega uno de ellos al predio, sino se crea 1 nuevo
		if(!this->playersToWait->empty()){
			Player* playerToGame = this->playersToWait->back();
			this->playersToWait->pop_back();
			this->playersToGame->push_back(playerToGame);
		}else{
			Player* player =  new Player(this->generateId());
			this->playersToGame->push_back(player);
		}

	}else{
		Player* playerToWait =  new Player(this->generateId());
		this->playersToWait->push_back(playerToWait);
	}
}



/*
* remueve a todos aquellos jugadores que alcanzaron la cantidad maxima de partidos
*/
/*
void PlayerManager::removePlayersWithGamesCompleted(){
	std::vector<Player*>::iterator it;
	Player * player;

	for(it = this->playersToGame->begin();it != this->playersToGame->end();it++){
		if((*it)->getGamesPlayed() == this->maxMatchesPerPlayer){
			//sacar del predio definitivamente al jugador
			player = (*it);

			//opcion1
			player->gameOver();
			this->playersToWait->push_back(player);
			this->playersToGame->erase(it);

			//opcion 2
			//this->playersToGame->erase(it);
			//delete player;
			
			log("Jugador ha completado los partidos permitidos, jugador con id ",(*it)->getId(),INFORMATION);

		}else if(player->getGamesPlayed() > this->maxMatchesPerPlayer) {
			log("Jugador ha jugado mas partidos de los permitidos, jugador con id ",(*it)->getId(),ERROR);
		}
	}
}

*/


struct messagePlayer* PlayerManager::readFifoPlayerManager(){
	struct messagePlayer *buff = new messagePlayer;
	memset(buff,0,sizeof(messagePlayer));

	int result = this->channelToRead->leer(buff,sizeof(messagePlayer));

	if(result == -1){
		log("No se pudo realizar la lectura del fifo ",__FILE__, __LINE__, ERROR);
	}else if (result != sizeof(messagePlayer)){
		log("Se ha leido una cantidad erronea de bytes del fifo ", __FILE__, __LINE__, ERROR);
	}
	
	return buff;
}


/*
* escribe un jugador al teamManager si este se encuentra libre
*/
void PlayerManager::writeFifoTeamManager(){
	std::vector<Player*>::iterator it;
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


//para notificar a teamManager que no se completo el partido del jugador
void PlayerManager::writeMessagePlayer(struct messagePlayer* message){

	int result = this->channelToWrite->escribir(message,sizeof(messagePlayer));

		if(result == -1){
			log("No se pudo realizar la escritura en el fifo, ", __FILE__, __LINE__, ERROR);
		}else if (result != sizeof(messagePlayer)){
			log("Se ha escrito una cantidad erronea de bytes en el fifo, ", __FILE__, __LINE__, ERROR);
		}

}




/*
*
*
*
*Falta definir el remove
*
*
*/
void PlayerManager::removePlayerToGame(){
	if(!this->playersToGame->empty()){
		int position = getRandomBetween(0,this->playersToGame->size());
		std::cout<<"sale el jguador en la pos "<<position<<std::endl;		
	}
}


/**
 * 
 * 
 * tiene que ver con la eliminacion para implementarlo
 * 
 * falta ver.....
 * **/
void PlayerManager::updateMatchesPlayer(int idPlayer){
	std::vector<Player*>::iterator it;
	bool found = false;
	Player* player;

	while(it != this->playersToGame->end() || !found){
		player = (*it);
		it++;
		if(player->getId() == idPlayer){
			found = true;
			player->addGamePlayed();
			player->endGame();	//el jugador esta libre para volver a jugar
			this->evaluteGamesCompletedPlayer(it);
		}
	}
}


/**
 * evalua si un jugador ha completado la maxima cantidad de partidos
 * el jugador se encuentra en el predio osea en playersToGame
 * si los ha completado se saca del predio
 **/ 

void PlayerManager::evaluteGamesCompletedPlayer(std::vector<Player*>::iterator it){

	Player *player = (*it);
	if(player->getGamesPlayed() == this->maxMatchesPerPlayer ){
		//opcion1
		//player->gameOver();
		//this->playersToWait->push_back(player);
		//this->playersToGame->erase(it);

		//opcion 2
		this->playersToGame->erase(it);
		log("Jugador ha completado los partidos permitidos, jugador con id ",(*it)->getId(),INFORMATION);		

	}else if(player->getGamesPlayed() > this->maxMatchesPerPlayer){
		log("Jugador ha jugado mas partidos de los permitidos, jugador con id ",(*it)->getId(),ERROR);
	}

	delete player;
}

