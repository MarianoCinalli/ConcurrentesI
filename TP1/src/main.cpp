#include <iostream>
#include <string>
#include "tools/logger.h"
#include "tools/Constants.h"
#include <unistd.h>


#include "tools/ProcessSpawner.h"
#include "CommandManager.h"
#include "TeamManager.h"

#include "PlayerManager.h"

// Constants ------------------------------------------------------
int LOG_MIN_LEVEL = 1;
std::ofstream LOG_FILE_POINTER;
// Constants ------------------------------------------------------


void hacerOtraCosa(){
    PlayerManager *playerManager = new PlayerManager(10,5);
    std::cout<<"construi hacer otra cosa"<<std::endl;
	playerManager->execute();
}


void hacerAlgo(){
    CommandManager *commandManager = new CommandManager();
    std::cout<<"construi hacer algo "<<std::endl;
	commandManager->execute();
}

typedef void (*functiontype)();

int main(int argc, char* argv[]) {
    // Initialization
    srand(time(NULL)); // Init seed for random
    LOG_FILE_POINTER.open("beachVoley.log", std::ofstream::app);
    // End Initialization
    logSessionStarted();
    // Main body

    //testChoto
    //CommandManager *commandManager = new CommandManager();
   	// void (*f1)();
    //f1 = commandManager->execute();

    std::vector<functiontype> *functions = new std::vector<functiontype>();
    functions->push_back(hacerAlgo);
    functions->push_back(hacerOtraCosa);
    ProcessSpawner *processSpawner = new ProcessSpawner();
    processSpawner->spawnProcesses(functions);
    
    delete functions;
    // Team Manager Test
    //TeamManager *teamManager = new TeamManager();

    // End Main body
	logSessionFinished();
	return 0;
}
