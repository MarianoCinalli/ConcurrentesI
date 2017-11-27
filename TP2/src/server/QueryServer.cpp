#include "server/QueryServer.h"

QueryServer::QueryServer(const std::string& file,const char letter,int clientType){
    this->mQueue = new Queue(file,letter);
    this->mType = getpid();
    this->reciverType = clientType;
    this->finalized = false;
}

QueryServer::~QueryServer(){
    this->mQueue->destroy();
    delete this->mQueue;
}

void QueryServer::execute(){
    //aceptando la conexión
    struct messageConection conection;
    conection.mtype = this->reciverType;
    conection.senderType = this->mType;
    this->mQueue->write(static_cast<const void*>(&conection),sizeof(messageConection));

    
    struct messageQuery message;
    while(!finalized){
        this->mQueue->read(this->mType,static_cast<void*>(&message),sizeof(messageQuery));
        log("Consulta del cliente con id: ",this->reciverType,INFORMATION);
        this->parseMessage(message);  
    }
}

void QueryServer::parseMessage(struct messageQuery message){
    switch(message.queryType){
        
        case servicesQuery::SERVICE_WEATHER :
            std::cout<<"Consultarlo con el servicio del clima"<<std::endl;
            std::cout<<"Consulta: "<< message.query<<std::endl;
            break;
        
        case servicesQuery::SERVICE_EXCHANGERATE :
            std::cout<<"Consultarlo con el servicio de tiepo de cambio"<<std::endl;
            std::cout<<"Consulta: "<< message.query<<std::endl;
            break;
    
        case servicesQuery::END_CONECTION :
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
