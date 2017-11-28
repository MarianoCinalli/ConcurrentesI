#include <iostream>
#include <string>
#include "tools/logger.h"


#include"client/Client.h"
#include"server/Server.h"

// Constants ------------------------------------------------------
int LOG_MIN_LEVEL = 1;
std::ofstream LOG_FILE_POINTER;
std::string file = "test.txt";
char letter = 'a';
// Constants ------------------------------------------------------


void client(){
    Client* client = new Client(file,letter);
    client->execute();
    delete client;
}

void administrator(){
    std::cout<<"Soy un administrador"<<std::endl;
}

void server(){
    Server* server = new Server(file,letter,0,0);
    server->execute();
    delete server; 
}


int main(int argc, char* argv[]) {
    // Initialization
    srand(time(NULL)); // Init seed for random
    LOG_FILE_POINTER.open("beachVoley.log", std::ofstream::app);
    // End Initialization
    logSessionStarted();

    char imput;
    std::cout<<"Ingrese la opción "<<std::endl;
    std::cin>>imput;

    switch(imput){
        case '1':
            server();
            break;

        case '2':
            client();
            break;

        case '3':
            administrator();
            break;

        default:     
            std::cout<<"Opción Incorrecta"<<std::endl;
    }

/*    pid_t pid;
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
*/  
	logSessionFinished();
	return 0;
}