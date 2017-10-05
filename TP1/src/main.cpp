#include <iostream>
#include <string>
#include "tools/logger.h"
#include "tools/Constants.h"
#include <unistd.h>

// Constants ------------------------------------------------------
int LOG_MIN_LEVEL = 1;
std::ofstream LOG_FILE_POINTER;
// Constants ------------------------------------------------------

int main(int argc, char* argv[]) {
    // Initialization
    srand(time(NULL)); // Init seed for random
    LOG_FILE_POINTER.open("beachVoley.log", std::ofstream::app);
    // End Initialization
    logSessionStarted();
    // Main body
    // End Main body
	logSessionFinished();
	return 0;
}
