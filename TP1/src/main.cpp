#include <iostream>
#include <string>
#include "tools/logger.h"
#include <unistd.h>

#include "tools/ProcessSpawner.h"
#include "CommandManager.h"
#include "TeamManager.h"
#include "MatchManager.h"
#include "../headers/playerManager/PlayerManager.h"

// TODO: Delete when finished.
#include "tools/utilFunctions.h"
#include <sys/wait.h>
#include "tools/ProcessSpawner.h"
#include "court/Court.h"

void openCourt() {
	Court* court = new Court();
	court->runUntilThereAreNoMatchesLeft();
	delete(court);
}
// -------------------------------

// Constants ------------------------------------------------------
int LOG_MIN_LEVEL = 1;
std::ofstream LOG_FILE_POINTER;
// Constants ------------------------------------------------------


void executePlayerManager(){
    PlayerManager *playerManager = new PlayerManager(10,5);
    log("INICIO DEL PLAYER_MANAGER",INFORMATION);
    playerManager->execute();
    delete playerManager;
}


void executeCommandManager(){
    CommandManager *commandManager = new CommandManager();
    log("INICIO DEL COMMAND_MANAGER",INFORMATION);
    commandManager->execute();
    delete commandManager;
}

void executeTeamManager(){
    TeamManager *teamManager = new TeamManager();
    log("INICIO DEL TEAM_MANAGER",INFORMATION);
    teamManager->execute();
    delete teamManager;
}

void executeMatchManager(){
    MatchManager *matchManager = new MatchManager();
    log("INICIO DEL MATCH_MANAGER",INFORMATION);
    matchManager->execute();
    delete matchManager;
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
/*
    // Main body

    // TODO: Delete when finished. -------------------------
	ProcessSpawner* spawner = new ProcessSpawner();
	pid_t newProcessPid = spawner->spawnProcess(&openCourt);
	int status;
	waitpid(newProcessPid, &status, WUNTRACED);
	// -----------------------------------------------------

    // End Main body
*/

	logSessionFinished();
	return 0;
}
