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

    std::string file = "test.txt";
    char letter = 'a';

    pid_t pid;
    pid = fork();

    if(pid == 0){
        Client* client = new Client(file,letter);
        client->execute();
        delete client;
        exit(0);
    }else{
        Server* server = new Server(file,letter,0,0);
        server->execute();
        delete server;
    }
    
	logSessionFinished();
	return 0;
}
