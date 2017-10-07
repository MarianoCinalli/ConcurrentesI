#include <iostream>
#include <string>
#include "tools/logger.h"
#include "tools/Constants.h"
#include <unistd.h>


#include "tools/ProcessSpawner.h"
#include "CommandManager.h"

// Constants ------------------------------------------------------
int LOG_MIN_LEVEL = 1;
std::ofstream LOG_FILE_POINTER;
// Constants ------------------------------------------------------


void hacerAlgo(){
	CommandManager *commandManager = new CommandManager();
	commandManager->execute();
}

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
    ProcessSpawner *processSpawner = new ProcessSpawner();
    processSpawner->spawnProcess(&hacerAlgo);


    // End Main body
	logSessionFinished();
	return 0;
}
