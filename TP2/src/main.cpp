#include <iostream>
#include <string>
#include "tools/logger.h"


#include"client/Client.h"
#include"server/Server.h"

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

    std::string file = "main.cpp";
    char letter = 'a';

    Server* server = new Server(file,letter,0,0);
    Client* client = new Client(file,letter);
    
	logSessionFinished();
	return 0;
}
