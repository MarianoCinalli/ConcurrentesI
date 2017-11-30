#include "server/ServerAbstract.h"
#include "services/ServicesAdministrator.h"
extern ServicesAdministrator* ADMINISTRATOR;

ServerAbstract::ServerAbstract(const std::string& file,const char letter,int clientType){
    this->mQueue = new Queue(file,letter);
    this->mType = getpid();
    this->reciverType = clientType;
    this->finalized = false;
    log(SERVER_ABSTRACT_NAME + " :Inicializador Server Abstract, para atender a cliente con id: ",clientType,INFORMATION);
}

ServerAbstract::~ServerAbstract(){
    this->deleteResources();
}

void ServerAbstract::deleteResources() {
    delete this->mQueue;
}


std::string ServerAbstract::logMemberVariables(){
    std::string registerLog = "mType: " + std::to_string(this->mType) + 
    " reciverType: " + std::to_string(this->reciverType);
    return registerLog;
}


void ServerAbstract::solveQueryWeather(struct messageQuery message){
    std::cout<<"Consultarlo con el servicio del clima"<<std::endl;
    std::cout<<"Consulta Recibida: "<< message.query<<std::endl;
    log(SERVER_ABSTRACT_NAME + " :solveQueryWeather para cliente con id: ",this->reciverType,INFORMATION);
    struct messageReplyWeatherService reply;
/*
    reply.mtype = this->reciverType;
    reply.temperature = 20;
    reply.pressure = 20;
	reply.humidity = 20;
    reply.errorId = 0;
    this->mQueue->write(static_cast<const void*>(&reply),sizeof(messageReplyWeatherService));
*/
    ADMINISTRATOR->sendReadMessageToWeatherService(this->mType, message.query);
    reply = ADMINISTRATOR->recieveMessageFromWeatherService(this->mType);

    if (reply.errorId == NO_ERROR) {
        log(SERVER_ABSTRACT_NAME + " : se responde la consulta satisfactoriamente del Servicio del clima",INFORMATION);
    } else {
        log(SERVER_ABSTRACT_NAME + " : la consulta del clima no se pudo responder",ERROR);
    }

    reply.mtype = this->reciverType;
    this->mQueue->write(static_cast<const void*>(&reply),sizeof(messageReplyWeatherService));
    log(SERVER_ABSTRACT_NAME + " :Envio respuesta del clima al cliente con id: ",this->reciverType,INFORMATION);
}

void ServerAbstract::solveQueryExchangeRate(struct messageQuery message){
    std::cout<<"Consultarlo con el servicio de tiepo de cambio"<<std::endl;
    std::cout<<"Consulta Recibida: "<< message.query<<std::endl;
    log(SERVER_ABSTRACT_NAME + " :solveQueryExchangeRate para cliente con id: ",this->reciverType,INFORMATION);
    struct messageReplyExchangeRatesService reply;
/*
    reply.mtype = this->reciverType;    
    reply.exchangeRate = 10;
    reply.errorId = 0;    
*/

    ADMINISTRATOR->sendReadMessageToCurrencyExchangeService(this->mType, message.query);
    reply = ADMINISTRATOR->recieveMessageFromCurrencyExchangeService(this->mType);

    if (reply.errorId == NO_ERROR) {
        log(SERVER_ABSTRACT_NAME + " : se responde la consulta satisfactoriamente del Servicio de tipo de cambio",INFORMATION);
    } else {
        log(SERVER_ABSTRACT_NAME + " : la consulta de tipo de cambio no se pudo responder",ERROR);
    }

    reply.mtype = this->reciverType;
    this->mQueue->write(static_cast<const void*>(&reply),sizeof(messageReplyExchangeRatesService));
    log(SERVER_ABSTRACT_NAME + " :Envio respuesta de tipo de cambio al cliente con id: ",this->reciverType,INFORMATION);    
}