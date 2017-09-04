#include <iostream>
#include <string>
#include "tools/logger.h"

// Constants ------------------------------------------------------
int LOG_MIN_LEVEL = 1;
std::ofstream LOG_FILE_POINTER;
// Constants ------------------------------------------------------

int main(int argc, char* argv[]){
    std::string logFileName = "/var/tmp/beachVoley.log";
    LOG_FILE_POINTER.open(logFileName, std::ofstream::app);
    logSessionStarted();
	std::cout << "Hello World!" << std::endl;
	logSessionFinished();
	return 0;
}
