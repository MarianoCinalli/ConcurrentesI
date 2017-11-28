#include "client/Client.h"
#include "tools/logger.h"

Client::Client (const std::string& file,const char letter){
    this->mQueue = new Queue(file,letter);
    this->mType = getpid();
    //mensage por defecto para iniciar la conexión con el servidor
    this->reciverType = 1;
    this->finalize = false;
}

Client::~Client(){
    this->mQueue->destroy();
    delete this->mQueue;
}


void Client::execute(){
    std::cout<<"INICIA EL CLIENTE"<<std::endl;
    std::cout<<"OPCIONES:"<<std::endl;
    std::cout<<"PARA INCIAR LA CONEXIÓN INGRESAR LA OPCIÓN: 1"<<std::endl;
    std::cout<<"PARA CONSULTAR EL CLIMA INGRESAR LA OPCIÓN: 2"<<std::endl;
    std::cout<<"PARA CONSULTAR EL EL TIPO DE CAMBIO INGRESAR LA OPCIÓN: 3"<<std::endl;
    std::cout<<"PARA TERMINAR LA CONEXIÓN INGRESAR LA OPCIÓN: 0"<<std::endl;
    
    while(!this->finalize){
        int option;
        std::cin>>option;
        switch(option){
            case optionConnect:
                this->connect();
                break;
                
            case optionQueryWeather:
                this->solveQueryWeaher();
                break;

            case optionQueryExchangeRate:
                this->solveQueryExchangeRate();
                break;

            case optionDisconnect:
                this->disconnect();
                break;
        }
    }
}


void Client::solveQueryWeaher(){
    std::cout<<"INGRESE LA CIUDAD DE LA CUAL DESEA SABER EL CLIMA"<<std::endl;
    std::string city;
    std::cin>>city;
    struct messageReplyWeatherService reply = this->queryWeather(city);
    std::cout<<"Temperatura: "<<reply.temperature<<std::endl;
    std::cout<<"Presión: "<<reply.pressure<<std::endl;
    std::cout<<"Humedad: "<<reply.humidity<<std::endl;
}

void Client::solveQueryExchangeRate(){
    std::cout<<"INGRESE LA MONEDA DE LA CUAL DESEA SABER EL TIPO DE CAMBIO EN RELACION AL PESO ARGENTINO"<<std::endl;
    std::string currency;
    std::cin>>currency;
    struct messageReplyExchangeRatesService reply = this->queryExchangeRate(currency);
    std::cout<<"Tipo de Cambio: "<<reply.exchangeRate<<std::endl;
}


void Client::connect(){
    struct messageConection conection;
    conection.mtype = this->reciverType;
    conection.senderType = this->mType;
    this->mQueue->write(static_cast<const void*>(&conection),sizeof(messageConection));
    log("Conexión iniciada por el cliente con id: ",this->mType,INFORMATION);
    this->mQueue->read(this->mType,static_cast<void*>(&conection),sizeof(messageConection));
    log("Conexión aceptada pora el cliente con id: ",this->mType,INFORMATION);  
    this->reciverType = conection.senderType;
}


void Client::disconnect(){
    struct messageQuery message;
    message.mtype = this->reciverType;
    message.queryType = servicesQuery::END_CONECTION;
    message.query = "";
    this->mQueue->write(static_cast<const void*>(&message),sizeof(messageQuery));
    log("Terminando la conexión con el servidor, cliente con id: ",this->mType,INFORMATION);
} 

struct messageReplyWeatherService Client::queryWeather(std::string city){
    struct messageQuery message;
    message.mtype = this->reciverType;
    message.queryType = servicesQuery::SERVICE_WEATHER;
    message.query = city;
    this->mQueue->write(static_cast<const void*>(&message),sizeof(messageQuery));
    log("Consulta de Clima por parte del cliente con id: ",this->mType,INFORMATION);

    struct messageReplyWeatherService messageReply;
    memset(&messageReply,0,sizeof(messageReplyWeatherService));
    this->mQueue->read(this->mType,static_cast<void*>(&messageReply),sizeof(messageReplyWeatherService));
    log("Respuesta del Clima recibida por el cliente con id: ",this->mType,INFORMATION); 
    return messageReply;   
}

struct messageReplyExchangeRatesService Client::queryExchangeRate(std::string currency){
    struct messageQuery message;
    message.mtype = this->reciverType;
    message.queryType = servicesQuery::SERVICE_EXCHANGERATE;
    message.query = currency;
    this->mQueue->write(static_cast<const void*>(&message),sizeof(messageQuery));
    log("Consulta de Tipo de Cambio por parte del cliente con id: ",this->mType,INFORMATION);

    struct messageReplyExchangeRatesService messageReply;
    memset(&messageReply,0,sizeof(messageReplyExchangeRatesService));       
    this->mQueue->read(this->mType,static_cast<void*>(&messageReply),sizeof(messageReplyExchangeRatesService));
    log("Respuesta del Tipo de Cambio recibida por el cliente con id: ",this->mType,INFORMATION);
    return messageReply;             
}

std::string Client::logMemberVariables(){
    std::string registerLog = "mType: " + std::to_string(this->mType) + 
    " reciverType: " + std::to_string(this->reciverType);
    return registerLog;
}