#include "CommandManager.h"
#include <iostream>

//include "log.h"
#include "fifos/FifoEscritura.h"


CommandManager::CommandManager(){
	this->finalizedProcess = false;
}



void CommandManager::execute(){
	char value;
	while (!finalizedProcess){
		std::cin>>value;
		this->receiveCommand(value);
	}	

}



void CommandManager::receiveCommand(char command){

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
		case 'q' :
			this->finalizedProcess = true;
			break;

		default:
			std::cout<<"loggear comando erroneo"<<std::endl;

	}

}


void CommandManager::addPlayer(){
	std::cout<<"agregue un judador"<<std::endl;
}

void CommandManager::removePlayer(){
	std::cout<<"quite un judador"<<std::endl;
}

void CommandManager::raiseTide(){
	std::cout<<"levanta la marea"<<std::endl;
}


void CommandManager::lowTide(){
	std::cout<<"baja la marea"<<std::endl;
}	
