#include <iostream>
#include <string>
#include "tools/logger.h"
#include "tools/Constants.h"
#include <unistd.h>


#include "tools/ProcessSpawner.h"
#include "CommandManager.h"
#include "TeamManager.h"
#include "MatchManager.h"
#include "../headers/playerManager/PlayerManager.h"

// Constants ------------------------------------------------------
int LOG_MIN_LEVEL = 1;
std::ofstream LOG_FILE_POINTER;
// Constants ------------------------------------------------------


void executePlayerManager(){
    PlayerManager *playerManager = new PlayerManager(10,5);
    log("INICIO DEL PLAYER_MANAGER",INFORMATION);
	playerManager->execute();
}


void executeCommandManager(){
    CommandManager *commandManager = new CommandManager();
    log("INICIO DEL COMMAND_MANAGER",INFORMATION);
	commandManager->execute();
}

void executeTeamManager(){
    TeamManager *teamManager = new TeamManager();
    log("INICIO DEL TEAM_MANAGER",INFORMATION);
	teamManager->execute();
}

void executeMatchManager(){
    MatchManager *matchManager = new MatchManager();
    log("INICIO DEL MATCH_MANAGER",INFORMATION);
	matchManager->execute();
}


typedef void (*functiontype)();

int main(int argc, char* argv[]) {
    // Initialization
    srand(time(NULL)); // Init seed for random
    LOG_FILE_POINTER.open("beachVoley.log", std::ofstream::app);
    // End Initialization

    std::cout<<"COMIENZA EJECUCION"<<std::endl;

    logSessionStarted();
    std::vector<functiontype> *functions = new std::vector<functiontype>();
    functions->push_back(executeCommandManager);
    functions->push_back(executePlayerManager);
    functions->push_back(executeTeamManager);
    functions->push_back(executeMatchManager);

    ProcessSpawner *processSpawner = new ProcessSpawner();
    processSpawner->spawnProcesses(functions);
    
    delete functions;
    // Team Manager Test
    //TeamManager *teamManager = new TeamManager();

    // End Main body
	logSessionFinished();
	return 0;
}
