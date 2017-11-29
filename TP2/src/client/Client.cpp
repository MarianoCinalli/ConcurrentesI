#include "client/Client.h"
#include "tools/logger.h"

Client::Client (const std::string& file,const char letter) : ClientAbstract(file,letter, typesClientConections::CLIENT_CONECTION) { 
    this->pid = getpid();
    log(CLIENT_NAME + " " + std::to_string(this->pid) + " : Inicializador cliente con id: ",INFORMATION);
}

Client::~Client(){
}

void Client::execute(){
    
    bool connect = false;
    std::cout<<"PARA INCIAR LA CONEXIÓN INGRESAR LA OPCIÓN: 1"<<std::endl;
    while(!connect){
        char option = '\0';
        std::cin>>option;
        if(option == optionConnect){
            this->connect();
            connect = true;
        }else{
             std::cout<<"OPCION INCORRECTA"<<std::endl;
        }
    }

    std::cout<<"CONEXIÓN INICIADA"<<std::endl;
    

    while(!this->getFinalizeProcess()){

        std::cout<<"-----------------------OPCIONES:--------------------------"<<std::endl;
        std::cout<<"PARA CONSULTAR EL CLIMA INGRESAR LA OPCIÓN: 2"<<std::endl;
        std::cout<<"PARA CONSULTAR EL EL TIPO DE CAMBIO INGRESAR LA OPCIÓN: 3"<<std::endl;
        std::cout<<"PARA TERMINAR LA CONEXIÓN INGRESAR LA OPCIÓN: 0"<<std::endl;
        std::cout<<"----------------------------------------------------------"<<std::endl;

        char option = '\0';
        std::cin>>option;
    
        switch(option){

            case optionQueryWeather:
                this->solveQueryWeaher();
                break;

            case optionQueryExchangeRate:
                this->solveQueryExchangeRate();
                break;

            case optionDisconnect:
                this->disconnect();
                break;

            default :
                 std::cout<<"opcion ingresada: "<<option<<" incorrecta"<<std::endl;
        }
    }
}

struct messageReplyWeatherService Client::queryWeather(std::string city){
    struct messageQuery message;
    message.mtype = this->reciverType;
    message.queryType = servicesQuery::SERVICE_WEATHER;
    strcpy(message.query,city.c_str());
    std::cout<<"lo que va a enviar el cliente: "<<message.query<<std::endl;        
    this->mQueue->write(static_cast<const void*>(&message),sizeof(messageQuery));
    log(CLIENT_NAME + " :Consulta de Clima por parte del cliente con id: ",this->mType,INFORMATION);

    struct messageReplyWeatherService messageReply;
    memset(&messageReply,0,sizeof(messageReplyWeatherService));
    this->mQueue->read(this->mType,static_cast<void*>(&messageReply),sizeof(messageReplyWeatherService));
    log(CLIENT_NAME + " :Respuesta del Clima recibida por el cliente con id: ",this->mType,INFORMATION); 
    return messageReply;   
}

struct messageReplyExchangeRatesService Client::queryExchangeRate(std::string currency){
    struct messageQuery message;
    message.mtype = this->reciverType;
    message.queryType = servicesQuery::SERVICE_EXCHANGERATE;
    strcpy(message.query,currency.c_str());  
    this->mQueue->write(static_cast<const void*>(&message),sizeof(messageQuery));
    log(CLIENT_NAME + " :Consulta de Tipo de Cambio por parte del cliente con id: ",this->mType,INFORMATION);

    struct messageReplyExchangeRatesService messageReply;
    memset(&messageReply,0,sizeof(messageReplyExchangeRatesService));       
    this->mQueue->read(this->mType,static_cast<void*>(&messageReply),sizeof(messageReplyExchangeRatesService));
    log(CLIENT_NAME + " :Respuesta del Tipo de Cambio recibida por el cliente con id: ",this->mType,INFORMATION);
    return messageReply;             
}
