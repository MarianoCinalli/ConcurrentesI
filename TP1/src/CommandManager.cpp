#include "CommandManager.h"
#include <iostream>

//include "log.h"

CommandManager::CommandManager(){
	this->finalizedProcess = false;
	this->fifoManagerPlayer = new FifoEscritura(FIFO_WRITE_COMMAND_TO_PLAYER);
	log(COMMAND_MANAGER_NAME + " Se construyo FIFO de escritura " + FIFO_WRITE_COMMAND_TO_PLAYER,INFORMATION);
	this->fifoTide = new FifoEscritura(FIFO_WRITE_COMMAND_TO_COURT);
	log(COMMAND_MANAGER_NAME + " Se construyo FIFO de escritura " + FIFO_WRITE_COMMAND_TO_COURT,INFORMATION);

}

void CommandManager::execute(){
	char value;
	while (!finalizedProcess){
		std::cin>>value;
		this->receiveCommand(value);
	}	
	log(COMMAND_MANAGER_NAME + " : El proceso CommandManager finaliza correctamente ",INFORMATION);
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
			log(COMMAND_MANAGER_NAME + " : Finaliza ",INFORMATION);
			messagePlayer *player = new messagePlayer;
			player->idPlayer = 10;
			player->status = CommandType::killType;
			this->fifoManagerPlayer->escribir(static_cast<const void*> (player), sizeof(player));
			break;
		}

		default:
			std::cout<<"loggear comando erroneo"<<std::endl;

	}

}

void CommandManager::addPlayer(){
	std::cout<<"Comando agregar jugador"<<std::endl;
	log(COMMAND_MANAGER_NAME + " : Se agrego un judador ",INFORMATION);
	messagePlayer *player = new messagePlayer;
	player->idPlayer = 0;
	player->status = CommandType::addType;
	this->fifoManagerPlayer->escribir(static_cast<const void*> (player), sizeof(player));
}

void CommandManager::removePlayer(){
	std::cout<<"Comando quitar un judador"<<std::endl;
	log(COMMAND_MANAGER_NAME + " : Se quito un judador ",INFORMATION);
	messagePlayer *player = new messagePlayer;
	player->idPlayer = 0;
	player->status = CommandType::removeType;
	this->fifoManagerPlayer->escribir(static_cast<const void*> (player), sizeof(player));
}

void CommandManager::raiseTide(){
	std::cout<<"Comando levantar la marea"<<std::endl;
	log(COMMAND_MANAGER_NAME + " : Se levanta la marea ",INFORMATION);
	messageTide *tide = new messageTide;
	tide->status = TideType::raiseType;
	this->fifoTide->escribir(static_cast<const void*> (tide), sizeof(tide));
}

void CommandManager::lowTide(){
	std::cout<<"Comando baja la marea"<<std::endl;
	log(COMMAND_MANAGER_NAME + " : Se baja la marea ",INFORMATION);
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