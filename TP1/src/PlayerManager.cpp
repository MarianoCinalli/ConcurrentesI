#include "PlayerManager.h"

PlayerManager::PlayerManager(int maxPlayersVillage, int maxMatchesPerPlayer){
	this->channelToRead = new FifoLectura(FILE_FIFO_READ);
	this->channelToWrite = new FifoEscritura(FILE_FIFO_WRITE);
	this->channelToRead->abrir();
	this->channelToWrite->abrir();	
	this->playersToGame = new std::vector<Player*>();
	this->playersToWait = new std::vector<Player*>();
	this->idPlayer = 0;
	this->maxPlayersVillage = maxPlayersVillage;
	this->maxMatchesPerPlayer = maxMatchesPerPlayer;
	this->finalizedProcess = false;
}

PlayerManager::~PlayerManager(){
	delete this->channelToRead;
	delete this->channelToWrite;
	delete this->playersToGame;
	delete this->playersToWait;
}

int PlayerManager::generateId(){
	return this->idPlayer++;
}



//retorno el id del player seleccionado para salir del predio
//falta
void PlayerManager::selectPlayerToRemove(){
	int position = getRandomBetween(0,this->playersToGame->size());
	std::cout<<"sale el jguador "<<position<<std::endl;
}


//falta terminar parser
void PlayerManager::execute(){
	while(!this->finalizedProcess){
		struct messagePlayer *message = this->readFifoPlayerManager();
		std::cout<<"leiiiiii "<<message->idPlayer<<std::endl;
		if(message->status == CommandType::kill)

		this->writeFifoTeamManager();		
	}
}


void PlayerManager::createPlayer(){
	Player *player =  new Player(this->generateId());
	
	if ((int)this->playersToGame->size() <= this->maxPlayersVillage){
		this->playersToGame->push_back(player);
	}else{
		this->playersToWait->push_back(player);
	}
}




void PlayerManager::removePlayersWithGamesCompleted(){
	std::vector<Player*>::iterator it;
	for(it = this->playersToGame->begin();it != this->playersToGame->end();it++){
		if((*it)->getGamesPlayed() == this->maxMatchesPerPlayer){
			//sacar del predio definitivamente al jugador
			(*it)->gameOver();
		}else if((*it)->getGamesPlayed() > this->maxMatchesPerPlayer) {
			//loggear error
			std::cout<<"Erro..... se jugaron mas partidos de los que debe"<<std::endl;
		}
	}
}



struct messagePlayer* PlayerManager::readFifoPlayerManager(){
	struct messagePlayer *buff = new messagePlayer;
	memset(buff,0,sizeof(messagePlayer));

	int result = this->channelToRead->leer(buff,sizeof(messagePlayer));

	if(result != -1){
		std::cout<<"todo bien..... al leer bufferPlayerManager"<<std::endl;
	}else{
		//loggear Error
		std::cout<<"Erro..... al leer bufferPlayerManager"<<std::endl;
	}
	return buff;
}


void PlayerManager::writeFifoTeamManager(){
	std::vector<Player*>::iterator it;
	for(it = this->playersToGame->begin();it != this->playersToGame->end();it++){
		if((*it)->isFree()){
			//envia un jugador al TeamManager
			struct messagePlayer *buff = new messagePlayer;
			memset(buff,0,sizeof(messagePlayer));
			buff->idPlayer = (*it)->getId();
			//buff->status = 0; //no importa
		
			int result = this->channelToWrite->escribir(buff,sizeof(messagePlayer));
			if(result != -1){
				std::cout<<"todo bien"<<std::endl;
			}else{
				//loggear Error
				std::cout<<"Erro..... en esribir bufferTeamManager"<<std::endl;
			}

		}
	}
}

