#include <iostream>
#include <string>
#include "tools/logger.h"
#include "tools/Constants.h"

// Constants ------------------------------------------------------
int LOG_MIN_LEVEL = 1;
std::ofstream LOG_FILE_POINTER;
// Constants ------------------------------------------------------

int main(int argc, char* argv[]){
    std::string logFileName = "/var/tmp/beachVoley.log";
    LOG_FILE_POINTER.open(logFileName, std::ofstream::app);
    logSessionStarted();
	std::cout << "Hello World!" << std::endl;
    std::cout << "MIN_SETS_DEFAULT is: " << MIN_SETS_DEFAULT << std::endl;
    std::cout << "MAX_SETS_DEFAULT is: " << MAX_SETS_DEFAULT << std::endl;
	logSessionFinished();

	return 0;
}
