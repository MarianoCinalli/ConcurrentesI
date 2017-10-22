#include "CommandManager.h"
#include <iostream>

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>

#include "courtManager/CourtManager.h"
#include "tools/utilFunctions.h"
#include "playerResult/ResultManager.h"
#include "tools/ProcessSpawner.h"

extern const char INITIAL_PARAMETERS[] = "initialParameter.json";

bool CommandManager::finalizedProcess =  false;

CommandManager::CommandManager(){
	//finalizedProcess = false;
	this->fifoManagerPlayer = new FifoEscritura(FIFO_WRITE_COMMAND_TO_PLAYERMANAGER);
	this->fifoTide = new FifoEscritura(FIFO_WRITE_COMMAND_TO_COURTMANAGER);
	log(COMMAND_MANAGER_NAME + " Se abrio FIFO de escritura " + FIFO_WRITE_COMMAND_TO_PLAYERMANAGER,INFORMATION);
	log(COMMAND_MANAGER_NAME + " Se abrio FIFO de escritura " + FIFO_WRITE_COMMAND_TO_COURTMANAGER,INFORMATION);
}



void CommandManager::executeCourtManager() {
    log("INICIO DEL COURT_MANAGER",INFORMATION);
	struct initialParameter *initialParameters = loadInitialParameters(INITIAL_PARAMETERS);
	CourtManager* courtManager = new CourtManager(initialParameters->rows, initialParameters->columns);
	courtManager->administrateCourts();
	delete(courtManager);
	delete initialParameters;
    log("FIN DEL COURT_MANAGER",INFORMATION);	
}



void CommandManager::executeResultManager(){
    log("INICIO DEL RESULT_MANAGER",INFORMATION);
    ResultManager *resultManager = new ResultManager();
    resultManager->execute();
    delete resultManager;
    log("FIN DEL RESULT_MANAGER",INFORMATION);
}


void CommandManager::execute(){

		
	pid_t pidCourt;
	pid_t pidResult;
	pid_t pid;
	int status;
	ProcessSpawner *processSpawner =  new ProcessSpawner();

	pidCourt = processSpawner->spawnProcess(CommandManager::executeCourtManager);
	pidResult = processSpawner->spawnProcess(CommandManager::executeResultManager);


	this->fifoManagerPlayer->abrir();
	this->fifoTide->abrir();
	char value;
	this->registerFunction();

	while (!finalizedProcess){
		std::cin>>value;
		this->receiveCommand(value);
		value = '\0';
	}


	pid = waitpid(pidResult,&status,WUNTRACED);	
	if(pid != -1){
		log(COMMAND_MANAGER_NAME + " : Proceso finalizado, su pid es ", pid, INFORMATION);
	}else{
		log(COMMAND_MANAGER_NAME + " : Error en la finalizacion del proceso ", INFORMATION);
	}
	flushLog();

	this->finalizeCourtManager();

	pid = waitpid(pidCourt,&status,WUNTRACED);	
	if(pid != -1){
		log(COMMAND_MANAGER_NAME + " : Proceso finalizado, su pid es ", pid, INFORMATION);
	}else{
		log(COMMAND_MANAGER_NAME + " : Error en la finalizacion del proceso ", INFORMATION);
	}
	flushLog();

	delete processSpawner;
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

//void CommandManager::finalizeResult(){
void CommandManager::finalize(){
	
	//finalizedProcess = true;
	log(COMMAND_MANAGER_NAME + " : El proceso CommandManager finaliza correctamente ",INFORMATION);
	messagePlayer *player = new messagePlayer;
	player->status = CommandType::killType;
	int status;
	status = this->fifoManagerPlayer->escribir(static_cast<const void*> (player), sizeof(player));
	if(status == -1){
		log(COMMAND_MANAGER_NAME + " :  **ERROR** no se pudo mandar fin de juego a PlayerManager",__FILE__, __LINE__, ERROR);
	}else if(status != sizeof(player)){
		log(COMMAND_MANAGER_NAME + " :  **ERROR** no se pudo mandar correctamente el fin de juego a PlayerManager",__FILE__, __LINE__, ERROR);
	}else{
		log(COMMAND_MANAGER_NAME + " :   se mando correctamente fin de juego a PlayerManager",INFORMATION);
	}
	delete player;
}


void CommandManager::finalizeCourtManager(){
	messageCourtManager* messageCourt = new messageCourtManager;
	messageCourt->operation = TideType::closeCourts;
	int status;
	status = this->fifoTide->escribir(static_cast<const void*> (messageCourt), sizeof(messageCourtManager));
	if(status == -1){
		log(COMMAND_MANAGER_NAME + " :  **ERROR** no se pudo mandar fin de juego a CourtManager",__FILE__, __LINE__, ERROR);
	}else if(status != sizeof(messageCourt)){
		log(COMMAND_MANAGER_NAME + " :  **ERROR** no se pudo mandar correctamente el fin de juego a CourtManager",__FILE__, __LINE__, ERROR);
	}else{
		log(COMMAND_MANAGER_NAME + " :   se mando correctamente fin de juego a CourtManager",INFORMATION);
	}

	delete messageCourt;
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
	log(COMMAND_MANAGER_NAME + " : Se registra el comando de levantar la marea ",INFORMATION);
	messageCourtManager* messageCourt = new messageCourtManager;
	messageCourt->operation = TideType::raiseType;
	this->fifoTide->escribir(static_cast<const void*> (messageCourt), sizeof(messageCourtManager));	
	delete messageCourt;
}

void CommandManager::lowTide(){
	std::cout<<"Comando bajar la marea"<<std::endl;
	log(COMMAND_MANAGER_NAME + " : Se registra el comando de baja de marea ",INFORMATION);
	messageCourtManager* messageCourt = new messageCourtManager;
	messageCourt->operation = TideType::lowType;
	this->fifoTide->escribir(static_cast<const void*> (messageCourt), sizeof(messageCourtManager));	
	delete messageCourt;
}	

CommandManager::~CommandManager(){
	log(COMMAND_MANAGER_NAME + " : Se liberan recursos ", INFORMATION);
	this->fifoManagerPlayer->cerrar();
	this->fifoTide->cerrar();
	delete this->fifoManagerPlayer;
	delete this->fifoTide;
}