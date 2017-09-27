#include <iostream>
#include <string>
#include "tools/logger.h"
#include "tools/Constants.h"
#include <unistd.h>

// Constants ------------------------------------------------------
int LOG_MIN_LEVEL = 1;
std::ofstream LOG_FILE_POINTER;
// Constants ------------------------------------------------------

void dosomething() { 
    log("Hola mi pid es ", getpid(), 3);
}

int main(int argc, char* argv[]) {
    // Initialization
    srand(time(NULL));
    std::string logFileName = "beachVoley.log";
    LOG_FILE_POINTER.open(logFileName, std::ofstream::app);
    // End Initialization
    logSessionStarted();
    // Main body
    // End Main body
	logSessionFinished();

	return 0;
}
