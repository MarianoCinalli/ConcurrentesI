#include <iostream>
#include <string>
#include "tools/logger.h"

//clients Admins Servers
#include"client/Client.h"
#include"client/Administrator.h"
#include"server/Server.h"

// Services
#include "services/ExchangeRatesService.h"
#include "services/WeatherService.h"
#include "services/ServicesAdministrator.h"

// Constants ------------------------------------------------------
int LOG_MIN_LEVEL = 1;
std::ofstream LOG_FILE_POINTER;
std::string file = "tp2.log";
char letter = 'A';
//------------------------------------------
ServicesAdministrator* ADMINISTRATOR;
int THISPROCESSPID;
int QUEUEID;
// Constants ------------------------------------------------------

// Spawn Services -------------------------------------------------
void executeExchangeRatesService(int parameters[]){
    log("main: Inicio del servicio de cambio de moneda.", INFORMATION);
    ExchangeRatesService* service = new ExchangeRatesService(parameters[0]);
    service->run();
    delete service;
    log("main: Fin del servicio de cambio de moneda.", INFORMATION);
};

void executeWeatherService(int parameters[]){
    log("main: Inicio del servicio de clima.", INFORMATION);
    WeatherService* service = new WeatherService(parameters[0]);
    service->run();
    delete service;
    log("main: Fin del servicio de clima.", INFORMATION);
};
// End Spawn Services ---------------------------------------------


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

void services(){
    key_t clave = ftok("tp2.log", 'A');
    QUEUEID = msgget(clave, 0777 | IPC_CREAT);

    ADMINISTRATOR = new ServicesAdministrator(QUEUEID);
    ADMINISTRATOR->spawnServices(&executeWeatherService, &executeExchangeRatesService);

    THISPROCESSPID = getpid();
}


int main(int argc, char* argv[]) {
    // Initialization
    LOG_FILE_POINTER.open("tp2.log", std::ofstream::app);
    // End Initialization
    logSessionStarted();

    std::cout<<"--------------------------OPCIONES:----------------"<<std::endl;
    std::cout<<"PARA EJECUTAR AL SERVIDOR INGRESE LA OPCION: 1"<<std::endl;
    std::cout<<"---------------------------------------------------"<<std::endl;
    std::cout<<"PARA EJECUTAR AL CLIENTE INGRESE LA OPCIÓN: 2"<<std::endl;
    std::cout<<"---------------------------------------------------"<<std::endl;
    std::cout<<"PARA EJECUTAR AL ADMINISTRADOR INGRESE LA OPCIÓN: 3"<<std::endl;
    std::cout<<"---------------------------------------------------"<<std::endl;


    char imput;
    std::cout<<std::endl;
    std::cin>>imput;

    switch(imput){
        case '1':
            services();
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
