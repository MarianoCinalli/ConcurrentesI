#include "server/AdministratorServer.h"

AdministratorServer::AdministratorServer(const std::string& file,const char letter,int clientType) : ServerAbstract(file, letter, clientType) {}

AdministratorServer::~AdministratorServer(){
}

void AdministratorServer::execute(){
    //aceptando la conexión
    struct messageConection conection;
    conection.mtype = this->reciverType;
    conection.senderType = this->mType;
    this->mQueue->write(static_cast<const void*>(&conection),sizeof(messageConection));

    
    struct messageQuery message;
    while(!finalized){
        this->mQueue->read(this->mType,static_cast<void*>(&message),sizeof(messageQuery));
        log(ADMINISTRATOR_SERVER_NAME + " : Consulta del cliente con id: ",this->reciverType,INFORMATION);
        this->parseMessage(message);  
    }
}

void AdministratorServer::parseMessage(struct messageQuery message){
    switch(message.queryType){
        
        case servicesQuery::SERVICE_WEATHER :
            this->solveQueryWeather(message);
            break;
        
        case servicesQuery::SERVICE_EXCHANGERATE :
            this->solveQueryExchangeRate(message);
            break;
    
        case servicesQuery::END_CONECTION :
            log(ADMINISTRATOR_SERVER_NAME + " : Se recibe el mensaje de finalización de conexión, cliente con id: ",this->reciverType,INFORMATION);
            std::cout<<"Fin de la conexión"<<std::endl; 
            this->finalized = true;
            break;
    }
}