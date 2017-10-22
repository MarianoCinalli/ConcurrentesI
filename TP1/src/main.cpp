#include <iostream>
#include <string>
#include <unistd.h>

#include "tools/ProcessSpawner.h"
#include "TeamManager.h"
#include "MatchManager.h"
#include "playerManager/PlayerManager.h"

#include "tools/logger.h"
#include "tools/utilFunctions.h"
#include "tools/ProcessSpawner.h"
#include "semaphores/Semaforo.h"
#include "fifos/FifoInitializer.h"

// Constants ------------------------------------------------------
const char INITIAL_PARAMETERS[] = "initialParameter.json";
int LOG_MIN_LEVEL = 1;
std::ofstream LOG_FILE_POINTER;
const std::string SEMAPHORE_NAME = "src/main.cpp";
Semaforo semaphoreFifoMatches(SEMAPHORE_NAME, 0);
// Constants ------------------------------------------------------

void executePlayerManager(){
    log("INICIO DEL PLAYER_MANAGER",INFORMATION);
    struct initialParameter *initialParameters = loadInitialParameters(INITIAL_PARAMETERS);
    PlayerManager *playerManager = new PlayerManager(initialParameters->maxPlayer , initialParameters->maxGamesByPlayer);
    playerManager->execute();
    delete initialParameters;
    delete playerManager;
    log("FIN DEL PLAYER_MANAGER",INFORMATION);
}

void executeTeamManager(){
    log("INICIO DEL TEAM_MANAGER",INFORMATION);
    TeamManager *teamManager = new TeamManager();
    teamManager->execute();
    delete teamManager;
    log("FIN DEL TEAM_MANAGER",INFORMATION);
}

void executeMatchManager(){
    log("INICIO DEL MATCH_MANAGER",INFORMATION);
    MatchManager *matchManager = new MatchManager();
    matchManager->execute();
    delete matchManager;
    log("FIN DEL MATCH_MANAGER",INFORMATION);
}



typedef void (*functiontype)();

int main(int argc, char* argv[]) {
    FifoInitializer* fifoInitializer = new FifoInitializer();
    fifoInitializer->createFifos();
    srand(time(NULL)); // Init seed for random
    LOG_FILE_POINTER.open("beachVoley.log", std::ofstream::app);
    std::cout<<"COMIENZA EJECUCION"<<std::endl;

    logSessionStarted();
    struct initialParameter* initialParameters = loadInitialParameters(INITIAL_PARAMETERS);
    log("INICIO DEL PROCESO PRINCIPAL",INFORMATION);
    ProcessSpawner *processSpawner = new ProcessSpawner();
    std::vector<functiontype> *functions = new std::vector<functiontype>();
    
    functions->push_back(executePlayerManager);
    functions->push_back(executeTeamManager);
    functions->push_back(executeMatchManager);
    processSpawner->spawnProcesses(functions);

    std::string lockFile = "/tmp/fifoMatches_lock";
    remove(lockFile.c_str());
    
    processSpawner->waitChilds();
    delete initialParameters;
    delete functions;
    log("FIN DEL PROCESO PRINCIPAL",INFORMATION);
    semaphoreFifoMatches.eliminar();
    fifoInitializer->deleteFifos();
    delete fifoInitializer;
	logSessionFinished();
	return 0;
}
