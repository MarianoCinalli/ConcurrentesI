#include <iostream>
#include <string>
#include "tools/logger.h"


#include"client/Client.h"
#include"client/Administrator.h"
#include"server/Server.h"

// Constants ------------------------------------------------------
int LOG_MIN_LEVEL = 1;
std::ofstream LOG_FILE_POINTER;
std::string file = "test.txt";
char letter = 'a';
// Constants ------------------------------------------------------


void client(){
    std::cout<<"SOY UN CLIENTE"<<std::endl;    
    Client* client = new Client(file,letter);
    client->execute();
    delete client;
    exit(0);
}

void administrator(){
    std::cout<<"SOY UN ADMINISTRADOR"<<std::endl;
    Administrator* admin = new Administrator(file,letter);
    admin->execute();
    delete admin;
    exit(0);   
}

void server(){
    std::cout<<"SOY UN SERVER"<<std::endl;    
    std::cout<<"ESCUCHANDO CONEXIONES....."<<std::endl;    
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
	logSessionFinished();
	return 0;
}