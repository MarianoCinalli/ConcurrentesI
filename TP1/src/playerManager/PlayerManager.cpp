#include "../headers/playerManager/PlayerManager.h"

PlayerManager::PlayerManager(int maxPlayersVillage, int maxMatchesPerPlayer){
	this->channelToRead = new FifoLectura(FILE_FIFO_READ);
	//this->channelToWrite = new FifoEscritura(FILE_FIFO_WRITE);	
	this->playersToGame = new std::vector<Player*>();
	this->playersToWait = new std::vector<Player*>();
	this->idPlayer = 0;
	this->maxPlayersVillage = maxPlayersVillage;
	this->maxMatchesPerPlayer = maxMatchesPerPlayer;
	this->finalizedProcess = false;
}

PlayerManager::~PlayerManager(){
	delete this->channelToRead;
	//delete this->channelToWrite;

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
	struct messagePlayer* message;
	while(!this->finalizedProcess){
		message = this->readFifoPlayerManager();
		this->parseMessage(message);
		//this->writeFifoTeamManager();		
	}
	log("El proceso PlayerManager finaliza correctamente ",INFORMATION);
}


//falta terminar parser
void PlayerManager::parseMessage(struct messagePlayer* message){

	switch (message->status){
		
		case CommandType::killType :
			this->finalizedProcess = true;
			break;

		case CommandType::addType :
			this->addPlayerToGame();
			break;

		case CommandType::removeType :
			this->removePlayerToGame();
			break;
	}
	
	delete message;
}


/*agrega un jugador, si el predio tiene capacidad se agrega al mismo
*si el predio no tiene capacidad se pone en espera
*/
void PlayerManager::addPlayerToGame(){

	if ((int)this->playersToGame->size() <= this->maxPlayersVillage){

		//si hay jugadores en espera se agregan al predio, sino se crea 1 nuevo
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



/*
* remueve a todos aquellos jugadores que alcanzaron la cantidad maxima de partidos
*/
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
			/*
			this->playersToGame->erase(it);
			delete player;
			*/
			log("Jugador ha completado los partidos permitidos, jugador con id ",(*it)->getId(),INFORMATION);

		}else if(player->getGamesPlayed() > this->maxMatchesPerPlayer) {
			log("Jugador ha jugado mas partidos de los permitidos, jugador con id ",(*it)->getId(),ERROR);
		}
	}
}




struct messagePlayer* PlayerManager::readFifoPlayerManager(){
	struct messagePlayer *buff = new messagePlayer;
	memset(buff,0,sizeof(messagePlayer));

	int result = this->channelToRead->leer(buff,sizeof(messagePlayer));

	if(result == -1){
		log("No se pudo realizar la lectura del fifo ",__FILE__,__LINE__);
	}else if (result != sizeof(messagePlayer)){
		log("Se ha leido una cantidad erronea de bytes del fifo ",__FILE__,__LINE__);
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
			struct messagePlayer *buff = new messagePlayer;
			memset(buff,0,sizeof(messagePlayer));
			buff->idPlayer = (*it)->getId();
		
			int result = this->channelToWrite->escribir(buff,sizeof(messagePlayer));

			if(result == -1){
				log("No se pudo realizar la escritura en el fifo ",__FILE__,__LINE__);
			}else if (result != sizeof(messagePlayer)){
				log("Se ha escrito una cantidad erronea de bytes en el fifo ",__FILE__,__LINE__);
			}

		}
	}
}
