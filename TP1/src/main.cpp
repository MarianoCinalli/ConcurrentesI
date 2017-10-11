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
    log("----------------",INFORMATION);
    log("INICIO DEL COURT",INFORMATION);
    log("----------------",INFORMATION);
	Court* court = new Court();
    court->runUntilThereAreNoMatchesLeft();
    delete(court);
    log("----------------",INFORMATION);
    log("FIN DEL COURT",INFORMATION);
    log("----------------",INFORMATION);
}
// -------------------------------

// Constants ------------------------------------------------------
int LOG_MIN_LEVEL = 1;
std::ofstream LOG_FILE_POINTER;
// Constants ------------------------------------------------------


void executePlayerManager(){
    log("-------------------------",INFORMATION);
    log("INICIO DEL PLAYER_MANAGER",INFORMATION);
    log("-------------------------",INFORMATION);
    PlayerManager *playerManager = new PlayerManager(10,5);
    playerManager->execute();
    delete playerManager;
    log("-------------------------",INFORMATION);
    log("FIN DEL PLAYER_MANAGER",INFORMATION);
    log("-------------------------",INFORMATION);
}


void executeCommandManager(){
    log("--------------------------",INFORMATION);
    log("INICIO DEL COMMAND_MANAGER",INFORMATION);
    log("--------------------------",INFORMATION);
    CommandManager *commandManager = new CommandManager();
    commandManager->execute();
    delete commandManager;
    log("--------------------------",INFORMATION);
    log("FIN DEL COMMAND_MANAGER",INFORMATION);
    log("--------------------------",INFORMATION);
}

void executeTeamManager(){
    log("-----------------------",INFORMATION);    
    log("INICIO DEL TEAM_MANAGER",INFORMATION);
    log("-----------------------",INFORMATION);
    TeamManager *teamManager = new TeamManager();
    teamManager->execute();
    delete teamManager;
    log("-----------------------",INFORMATION);    
    log("FIN DEL TEAM_MANAGER",INFORMATION);
    log("-----------------------",INFORMATION);  
}

void executeMatchManager(){
    log("------------------------",INFORMATION);    
    log("INICIO DEL MATCH_MANAGER",INFORMATION);
    log("------------------------",INFORMATION);
    MatchManager *matchManager = new MatchManager();
    matchManager->execute();
    delete matchManager;
    log("------------------------",INFORMATION);    
    log("FIN DEL MATCH_MANAGER",INFORMATION);
    log("------------------------",INFORMATION);
}


typedef void (*functiontype)();

int main(int argc, char* argv[]) {
    // Initialization
    srand(time(NULL)); // Init seed for random
    LOG_FILE_POINTER.open("beachVoley.log", std::ofstream::app);
    // End Initialization

    std::cout<<"COMIENZA EJECUCION"<<std::endl;

    logSessionStarted();
    log("----------------------------",INFORMATION);
    log("INICIO DEL PROCESO PRINCIPAL",INFORMATION);
    log("----------------------------",INFORMATION);
    std::vector<functiontype> *functions = new std::vector<functiontype>();
    functions->push_back(executeCommandManager);
    functions->push_back(executePlayerManager);
    functions->push_back(executeTeamManager);
    functions->push_back(executeMatchManager);
    functions->push_back(openCourt);

    ProcessSpawner *processSpawner = new ProcessSpawner();
    processSpawner->spawnProcesses(functions);
    
    delete functions;
    log("----------------------------",INFORMATION);
    log("FIN DEL PROCESO PRINCIPAL",INFORMATION);
    log("----------------------------",INFORMATION);

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
