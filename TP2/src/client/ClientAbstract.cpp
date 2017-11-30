#include "client/ClientAbstract.h"

ClientAbstract::ClientAbstract (const std::string& file,const char letter, enum typesClientConections typeClient){
    this->mQueue = new Queue(file,letter);
    this->mType = getpid();
    //mensage por defecto para iniciar la conexión con el servidor
    this->reciverType = 1;
    this->finalize = false;
    this->mTypeClient = typeClient;
    log(CLIENT_ABSTRACT_NAME + " :Conexión iniciada por el cliente con id: ",this->mType,INFORMATION);
}

ClientAbstract::~ClientAbstract(){
    delete this->mQueue;
}

void ClientAbstract::solveQueryWeaher(){
    std::cout<<"INGRESE LA CIUDAD DE LA CUAL DESEA SABER EL CLIMA"<<std::endl;
    std::string city;
    std::cin>>city;
    struct messageReplyWeatherService reply = this->queryWeather(city.c_str());
    
    if (reply.errorId == NO_ERROR) {
        std::cout<<"Temperatura: "<<reply.temperature<<std::endl;
        std::cout<<"Presión: "<<reply.pressure<<std::endl;
        std::cout<<"Humedad: "<<reply.humidity<<std::endl;
    } else {
        std::cout<<"Error la ciudad ingresada no existe"<<std::endl;
    }
    std::cout<<std::endl;
    std::cout<<std::endl;
}

void ClientAbstract::solveQueryExchangeRate(){
    std::cout<<"INGRESE LA MONEDA DE LA CUAL DESEA SABER EL TIPO DE CAMBIO EN RELACION AL PESO ARGENTINO"<<std::endl;
    std::string currency;
    std::cin>>currency;
    struct messageReplyExchangeRatesService reply = this->queryExchangeRate(currency.c_str());

    if (reply.errorId == NO_ERROR) {
        std::cout<<"Tipo de Cambio: "<<reply.exchangeRate<<std::endl;
    } else {
        std::cout<<"Error la moneda ingresada no existe"<<std::endl;
    }
    std::cout<<std::endl;
    std::cout<<std::endl;
}


void ClientAbstract::connect(){
    struct messageConection conection;
    conection.mtype = this->reciverType;
    conection.senderType = this->mType;
    conection.typeClient = this->mTypeClient;    
    this->mQueue->write(static_cast<const void*>(&conection),sizeof(messageConection));
    log(CLIENT_ABSTRACT_NAME + " :Conexión iniciada por el cliente con id: ",this->mType,INFORMATION);
    flushLog();
    this->mQueue->read(this->mType,static_cast<void*>(&conection),sizeof(messageConection));
    log(CLIENT_ABSTRACT_NAME + " :Conexión aceptada pora el cliente con id: ",this->mType,INFORMATION);  
    this->reciverType = conection.senderType;
}


void ClientAbstract::disconnect(){
    struct messageQuery message;
    message.mtype = this->reciverType;
    message.queryType = servicesQuery::END_CONECTION;
    strcpy(message.query,"");    
    this->mQueue->write(static_cast<const void*>(&message),sizeof(messageQuery));
    this->finalize = true;
    log(CLIENT_ABSTRACT_NAME + " :Terminando la conexión con el servidor, cliente con id: ",this->mType,INFORMATION);
} 

std::string ClientAbstract::logMemberVariables(){
    std::string registerLog = "mType: " + std::to_string(this->mType) + 
    " reciverType: " + std::to_string(this->reciverType);
    return registerLog;
}