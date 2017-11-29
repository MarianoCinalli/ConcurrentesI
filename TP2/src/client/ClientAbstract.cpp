#include "client/ClientAbstract.h"

ClientAbstract::ClientAbstract (const std::string& file,const char letter, enum typesClientConections typeClient){
    this->mQueue = new Queue(file,letter);
    this->mType = getpid();
    //mensage por defecto para iniciar la conexión con el servidor
    this->reciverType = 1;
    this->finalize = false;
    this->mTypeClient = typeClient;
}

ClientAbstract::~ClientAbstract(){
    delete this->mQueue;
}

void ClientAbstract::solveQueryWeaher(){
    std::cout<<"INGRESE LA CIUDAD DE LA CUAL DESEA SABER EL CLIMA"<<std::endl;
    std::string city;
    std::cin>>city;
    struct messageReplyWeatherService reply = this->queryWeather(city.c_str());
    std::cout<<"Temperatura: "<<reply.temperature<<std::endl;
    std::cout<<"Presión: "<<reply.pressure<<std::endl;
    std::cout<<"Humedad: "<<reply.humidity<<std::endl;
}

void ClientAbstract::solveQueryExchangeRate(){
    std::cout<<"INGRESE LA MONEDA DE LA CUAL DESEA SABER EL TIPO DE CAMBIO EN RELACION AL PESO ARGENTINO"<<std::endl;
    std::string currency;
    std::cin>>currency;
    struct messageReplyExchangeRatesService reply = this->queryExchangeRate(currency.c_str());
    std::cout<<"Tipo de Cambio: "<<reply.exchangeRate<<std::endl;
}


void ClientAbstract::connect(){
    struct messageConection conection;
    conection.mtype = this->reciverType;
    conection.senderType = this->mType;
    conection.typeClient = this->mTypeClient;    
    this->mQueue->write(static_cast<const void*>(&conection),sizeof(messageConection));
    log("Conexión iniciada por el cliente con id: ",this->mType,INFORMATION);
    flushLog();
    this->mQueue->read(this->mType,static_cast<void*>(&conection),sizeof(messageConection));
    log("Conexión aceptada pora el cliente con id: ",this->mType,INFORMATION);  
    this->reciverType = conection.senderType;
}


void ClientAbstract::disconnect(){
    struct messageQuery message;
    message.mtype = this->reciverType;
    message.queryType = servicesQuery::END_CONECTION;
    strcpy(message.query,"");    
    this->mQueue->write(static_cast<const void*>(&message),sizeof(messageQuery));
    this->finalize = true;
    log("Terminando la conexión con el servidor, cliente con id: ",this->mType,INFORMATION);
} 

struct messageReplyWeatherService ClientAbstract::queryWeather(std::string city){
    struct messageQuery message;
    message.mtype = this->reciverType;
    message.queryType = servicesQuery::SERVICE_WEATHER;
    strcpy(message.query,city.c_str());
    this->mQueue->write(static_cast<const void*>(&message),sizeof(messageQuery));
    log("Consulta de Clima por parte del cliente con id: ",this->mType,INFORMATION);

    struct messageReplyWeatherService messageReply;
    memset(&messageReply,0,sizeof(messageReplyWeatherService));
    this->mQueue->read(this->mType,static_cast<void*>(&messageReply),sizeof(messageReplyWeatherService));
    log("Respuesta del Clima recibida por el cliente con id: ",this->mType,INFORMATION); 
    return messageReply;   
}

struct messageReplyExchangeRatesService ClientAbstract::queryExchangeRate(std::string currency){
    struct messageQuery message;
    message.mtype = this->reciverType;
    message.queryType = servicesQuery::SERVICE_EXCHANGERATE;
    strcpy(message.query,currency.c_str());    
    this->mQueue->write(static_cast<const void*>(&message),sizeof(messageQuery));
    log("Consulta de Tipo de Cambio por parte del cliente con id: ",this->mType,INFORMATION);

    struct messageReplyExchangeRatesService messageReply;
    memset(&messageReply,0,sizeof(messageReplyExchangeRatesService));       
    this->mQueue->read(this->mType,static_cast<void*>(&messageReply),sizeof(messageReplyExchangeRatesService));
    log("Respuesta del Tipo de Cambio recibida por el cliente con id: ",this->mType,INFORMATION);
    return messageReply;             
}

std::string ClientAbstract::logMemberVariables(){
    std::string registerLog = "mType: " + std::to_string(this->mType) + 
    " reciverType: " + std::to_string(this->reciverType);
    return registerLog;
}