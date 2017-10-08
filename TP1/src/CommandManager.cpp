#include "CommandManager.h"
#include <iostream>

//include "log.h"

CommandManager::CommandManager(){
	this->finalizedProcess = false;
	this->fifoManagerPlayer = new FifoEscritura(FILE_FIFO_MANAGER_PLAYER);
	//this->fifoManagerPlayer->abrir();
	this->fifoTide = new FifoEscritura(FILE_FIFO_TIDE);
	//this->fifoTide->abrir();
	std::cout<<"construi fifo lectura command"<<std::endl;
}

void CommandManager::execute(){
	char value;
	std::cout<<"ejecutando command "<<std::endl;
	while (!finalizedProcess){
		std::cin>>value;
		this->receiveCommand(value);
	}	
}



void CommandManager::receiveCommand(char command){

	this->fifoManagerPlayer->abrir();

	switch (command){
		case 'a' :
			this->addPlayer();
			break;

		case 'r' :
			this->removePlayer();
			break;

		case '1' :
			this->raiseTide();
			break;

		case '2' :
			this->lowTide();
			break;

		//condición para testear
		case 'q' : {
			this->finalizedProcess = true;
			messagePlayer *player = new messagePlayer;
			player->idPlayer = 10;
			player->status = CommandType::killType;
			this->fifoManagerPlayer->escribir(static_cast<const void*> (player), sizeof(player));
			std::cout<<"Termino command Mannager"<<std::endl;
			break;
		}

		default:
			std::cout<<"loggear comando erroneo"<<std::endl;

	}

}

void CommandManager::addPlayer(){
	std::cout<<"agregue un judador"<<std::endl;
	messagePlayer *player = new messagePlayer;
	player->idPlayer = 0;
	player->status = CommandType::addType;
	this->fifoManagerPlayer->escribir(static_cast<const void*> (player), sizeof(player));
}

void CommandManager::removePlayer(){
	std::cout<<"quite un judador"<<std::endl;
	messagePlayer *player = new messagePlayer;
	player->idPlayer = 0;
	player->status = CommandType::removeType;
	this->fifoManagerPlayer->escribir(static_cast<const void*> (player), sizeof(player));
}

void CommandManager::raiseTide(){
	std::cout<<"levanta la marea"<<std::endl;
	messageTide *tide = new messageTide;
	tide->status = TideType::raiseType;
	this->fifoTide->escribir(static_cast<const void*> (tide), sizeof(tide));
}

void CommandManager::lowTide(){
	std::cout<<"baja la marea"<<std::endl;
	messageTide *tide = new messageTide;
	tide->status = TideType::lowType;
	this->fifoTide->escribir(static_cast<const void*> (tide), sizeof(tide));
}	

CommandManager::~CommandManager(){
	this->fifoManagerPlayer->cerrar();
	this->fifoTide->cerrar();
	delete this->fifoManagerPlayer;
	delete this->fifoTide;
}