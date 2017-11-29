#include "server/ServerAbstract.h"

ServerAbstract::ServerAbstract(const std::string& file,const char letter,int clientType){
    this->mQueue = new Queue(file,letter);
    this->mType = getpid();
    this->reciverType = clientType;
    this->finalized = false;
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
    struct messageReplyWeatherService reply;
    reply.mtype = this->reciverType;
    reply.temperature = 20;
    reply.pressure = 20;
	reply.humidity = 20;
    reply.errorId = 0;
    this->mQueue->write(static_cast<const void*>(&reply),sizeof(messageReplyWeatherService));
    log("Envio respuesta del clima al cliente con id: ",this->reciverType,INFORMATION);
}

void ServerAbstract::solveQueryExchangeRate(struct messageQuery message){
    std::cout<<"Consultarlo con el servicio de tiepo de cambio"<<std::endl;
    std::cout<<"Consulta Recibida: "<< message.query<<std::endl;
    struct messageReplyExchangeRatesService reply;
    reply.mtype = this->reciverType;    
    reply.exchangeRate = 10;
    reply.errorId = 0;    
    this->mQueue->write(static_cast<const void*>(&reply),sizeof(messageReplyExchangeRatesService));
    log("Envio respuesta de tipo de cambio al cliente con id: ",this->reciverType,INFORMATION);    
}