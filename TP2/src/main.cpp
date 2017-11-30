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


int main(int argc, char* argv[]) {
    // Initialization
    LOG_FILE_POINTER.open("tp2.log", std::ofstream::app);
    // End Initialization
    logSessionStarted();

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


    // -----------------------------------------------------
    key_t clave = ftok("tp2.log", 'A');
    if (clave < 0) {
        log("Error al generar un token para la cola de mensajes.", ERROR);
    }
    int queueId = msgget(clave, 0777 | IPC_CREAT);
    if (queueId < 0) {
        log("Error al crear la cola de mensajes.", ERROR);
    }

    ServicesAdministrator* administrator = new ServicesAdministrator(queueId);
    administrator->spawnServices(&executeWeatherService, &executeExchangeRatesService);

    int thisPocessPid = getpid();

    // READ AND REPLY EXAMPLES
    administrator->sendReadMessageToWeatherService(thisPocessPid, "buenos aires");

    messageReplyWeatherService weatherReply = administrator->recieveMessageFromWeatherService(thisPocessPid);
    if (weatherReply.errorId == NO_ERROR) {
        std::cout << "Temperatura: " << weatherReply.temperature <<
            " - Presion:" << weatherReply.pressure <<
            " - Humedad:" << weatherReply.humidity << std::endl;
    } else {
        std::cout << "Error" << std::endl;
    }

    administrator->sendReadMessageToCurrencyExchangeService(thisPocessPid, "dolar");

    messageReplyExchangeRatesService currencyReply = administrator->recieveMessageFromCurrencyExchangeService(thisPocessPid);
    if (currencyReply.errorId == NO_ERROR) {
        std::cout << "1 peso = " << currencyReply.exchangeRate << " dolares" << std::endl;
    } else {
        std::cout << "Error" << std::endl;
    }

    // UPDATE EXAMPLES
    administrator->sendUpdateMessageToCurrencyExchangeService(thisPocessPid, "peso uruguayo", 4); // Actualizar
    administrator->sendUpdateMessageToWeatherService(thisPocessPid, "ciudadInexistente", 23, 1, 100); // Crear porque no existe

    // ERASE EXAMPLES
    administrator->sendEraseMessageToCurrencyExchangeService(thisPocessPid, "euro");
    administrator->sendEraseMessageToWeatherService(thisPocessPid, "maipu");

    // END SERVICES EXAMPLE
    administrator->endServicesAndReturnWhenFinished();

    delete(administrator);
    // -----------------------------------------------------

    int status = msgctl(queueId, IPC_RMID, NULL);
    if (status < 0) {
        log("Error al eliminar la cola de mensajes.", ERROR);
    }
    logSessionFinished();
    return 0;
}
