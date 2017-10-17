#include "CommandManager.h"
#include <iostream>

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>



//include "log.h"

bool CommandManager::finalizedProcess =  false;

CommandManager::CommandManager(){
	//finalizedProcess = false;
	this->fifoManagerPlayer = new FifoEscritura(FIFO_WRITE_COMMAND_TO_PLAYERMANAGER);
	log(COMMAND_MANAGER_NAME + " Se construyo FIFO de escritura " + FIFO_WRITE_COMMAND_TO_PLAYERMANAGER,INFORMATION);
	this->fifoTide = new FifoEscritura(FIFO_WRITE_COMMAND_TO_COURTMANAGER);
	log(COMMAND_MANAGER_NAME + " Se construyo FIFO de escritura " + FIFO_WRITE_COMMAND_TO_COURTMANAGER,INFORMATION);

}

void CommandManager::execute(){
	char value;
	this->fifoManagerPlayer->abrir();
	this->registerFunction();

	while (!finalizedProcess){
		std::cin>>value;
		this->receiveCommand(value);
		value = '\0';
	}
	this->finalize();	
}

void CommandManager::sigInt_handler(int signum){
	if(signum == SIGINT){
		finalizedProcess = true;
	}
}


void CommandManager::registerFunction(){
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = CommandManager::sigInt_handler;
	sigemptyset ( &sa.sa_mask );	// inicializa la mascara de seniales a bloquear durante la ejecucion del handler como vacio
	sigaddset ( &sa.sa_mask,SIGINT );
	sigaction ( SIGINT,&sa,0 );	// cambiar accion de la senial
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
	}

}

void CommandManager::finalize(){
	//finalizedProcess = true;
	log(COMMAND_MANAGER_NAME + " : El proceso CommandManager finaliza correctamente ",INFORMATION);
	messagePlayer *player = new messagePlayer;
	player->status = CommandType::killType;
	this->fifoManagerPlayer->escribir(static_cast<const void*> (player), sizeof(player));
	delete player;
}


void CommandManager::addPlayer(){
	std::cout<<"Comando agregar jugador"<<std::endl;
	log(COMMAND_MANAGER_NAME + " : Se agrego un jugador ",INFORMATION);
	messagePlayer *player = new messagePlayer;
	player->idPlayer = 0;
	player->status = CommandType::addType;
	this->fifoManagerPlayer->escribir(static_cast<const void*> (player), sizeof(player));
	delete player;
}

void CommandManager::removePlayer(){
	std::cout<<"Comando quitar un judador"<<std::endl;
	log(COMMAND_MANAGER_NAME + " : Se quito un judador ",INFORMATION);
	messagePlayer *player = new messagePlayer;
	player->idPlayer = 0;
	player->status = CommandType::removeType;
	this->fifoManagerPlayer->escribir(static_cast<const void*> (player), sizeof(player));
	delete player;
}

void CommandManager::raiseTide(){
	std::cout<<"Comando levantar la marea"<<std::endl;
	log(COMMAND_MANAGER_NAME + " : Se levanta la marea ",INFORMATION);
	messageTide *tide = new messageTide;
	tide->status = TideType::raiseType;
	this->fifoTide->escribir(static_cast<const void*> (tide), sizeof(tide));
	delete tide;
}

void CommandManager::lowTide(){
	std::cout<<"Comando baja la marea"<<std::endl;
	log(COMMAND_MANAGER_NAME + " : Se baja la marea ",INFORMATION);
	messageTide *tide = new messageTide;
	tide->status = TideType::lowType;
	this->fifoTide->escribir(static_cast<const void*> (tide), sizeof(tide));
	delete tide;
}	

CommandManager::~CommandManager(){
	log(COMMAND_MANAGER_NAME + " : Se liberan recursos ", INFORMATION);
	this->fifoManagerPlayer->cerrar();
	this->fifoTide->cerrar();
	delete this->fifoManagerPlayer;
	delete this->fifoTide;
}