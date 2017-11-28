#include "server/QueryServer.h"

QueryServer::QueryServer(const std::string& file,const char letter,int clientType) : ServerAbstract(file, letter, clientType) {}

QueryServer::~QueryServer(){
    //deleteResources();
}

void QueryServer::execute(){
    //aceptando la conexión
    struct messageConection conection;
    conection.mtype = this->reciverType;
    conection.senderType = this->mType;
    this->mQueue->write(static_cast<const void*>(&conection),sizeof(messageConection));

    
    struct messageQuery *message = new struct messageQuery;
    while(!this->finalized){
        this->mQueue->read(this->mType,static_cast<void*>(message),sizeof(messageQuery));
        log(QUERY_SERVER_NAME + " : Consulta del cliente con id: ",this->reciverType,INFORMATION);
        this->parseMessage(*message);  
    }
    log(QUERY_SERVER_NAME + " : Finaliza query server",INFORMATION);
}

void QueryServer::parseMessage(struct messageQuery message){
    switch(message.queryType){
        
        case servicesQuery::SERVICE_WEATHER :
            this->solveQueryWeather(message);
            break;
        
        case servicesQuery::SERVICE_EXCHANGERATE :
            this->solveQueryExchangeRate(message);
            break;
    
        case servicesQuery::END_CONECTION :
            log(QUERY_SERVER_NAME + " : Se recibe el mensaje de finalización de conexión, cliente con id: ",this->reciverType,INFORMATION);
            std::cout<<"Fin de la conexión"<<std::endl; 
            this->finalized = true;
            break;
    }
}

std::string QueryServer::logMemberVariables(){
    std::string registerLog = "mType: " + std::to_string(this->mType) + 
    " reciverType: " + std::to_string(this->reciverType);
    return registerLog;
}


void QueryServer::solveQueryWeather(struct messageQuery message){
    std::cout<<"Consultarlo con el servicio del clima"<<std::endl;
    std::cout<<"Consulta: "<< message.query<<std::endl;
    struct messageReplyWeatherService reply;
    reply.mtype = this->reciverType;
    reply.temperature = 20;
    reply.pressure = 20;
	reply.humidity = 20;
    reply.errorId = 0;
    this->mQueue->write(static_cast<const void*>(&reply),sizeof(messageReplyWeatherService));
    log(QUERY_SERVER_NAME + " : Envio respuesta del clima al cliente con id: ",this->reciverType,INFORMATION);
}

void QueryServer::solveQueryExchangeRate(struct messageQuery message){
    std::cout<<"Consultarlo con el servicio de tiepo de cambio"<<std::endl;
    std::cout<<"Consulta: "<< message.query<<std::endl;
    struct messageReplyExchangeRatesService reply;
    reply.mtype = this->reciverType;    
    reply.exchangeRate = 10;
    reply.errorId = 0;    
    this->mQueue->write(static_cast<const void*>(&reply),sizeof(messageReplyExchangeRatesService));
    log(QUERY_SERVER_NAME + " : Envio respuesta de tipo de cambio al cliente con id: ",this->reciverType,INFORMATION);    
}