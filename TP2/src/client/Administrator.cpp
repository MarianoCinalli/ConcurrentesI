#include "client/Administrator.h"

Administrator::Administrator(const std::string& file,const char letter)
: ClientAbstract(file,letter,typesClientConections::ADMINISTRATOR_CONECTION){
}

Administrator::~Administrator(){
}

struct messageReplyExchangeRatesService Administrator::queryExchangeRate(std::string currency){
   
    struct messageAdministrator message;
    message.mtype = this->reciverType;
    message.typeService = servicesQuery::SERVICE_EXCHANGERATE;
    message.operationType = servicesOperations::SERVICE_OP_READ;
    strcpy(message.type,currency.c_str());

    std::cout<<"lo que va a enviar el administrador: "<<message.type<<std::endl;        
    this->mQueue->write(static_cast<const void*>(&message),sizeof(messageAdministrator));
    log(ADMINISTRATOR_NAME + " :Consulta del Tipo de cambio por el Administrador, moneda: "+currency,INFORMATION);

    struct messageReplyExchangeRatesService messageReply;
    memset(&messageReply,0,sizeof(messageReplyWeatherService));
    this->mQueue->read(this->mType,static_cast<void*>(&messageReply),sizeof(messageReplyExchangeRatesService));
    log(ADMINISTRATOR_NAME + " :Respuesta del Tipo de cambio al Administrador, moneda: "+currency,INFORMATION); 
    return messageReply;    
}


struct messageReplyWeatherService Administrator::queryWeather(std::string city){

    struct messageAdministrator message;
    message.mtype = this->reciverType;
    message.typeService = servicesQuery::SERVICE_WEATHER;
    message.operationType = servicesOperations::SERVICE_OP_READ;
    strcpy(message.type,city.c_str());

    std::cout<<"lo que va a enviar el administrador: "<<message.type<<std::endl;        
    this->mQueue->write(static_cast<const void*>(&message),sizeof(messageAdministrator));
    log(ADMINISTRATOR_NAME + " :Consulta del Clima por el Administrador, ciudad: "+city,INFORMATION);

    struct messageReplyWeatherService messageReply;
    memset(&messageReply,0,sizeof(messageReplyWeatherService));
    this->mQueue->read(this->mType,static_cast<void*>(&messageReply),sizeof(messageReplyWeatherService));
    log(ADMINISTRATOR_NAME + " :Respuesta del Clima al Administrador, ciudad: "+city,INFORMATION); 
    return messageReply;    
}

void Administrator::updateWeather(){
    std::string city;
    struct messageAdministrator message;
    struct messageReplyOperation reply;

    memset(&message,'\0',sizeof(messageAdministrator));
    memset(&reply,'\0',sizeof(messageReplyOperation));

    std::cout<<"INGRESE LA CIUDAD DONDE QUIERE ACTUALIZAR EL CLIMA"<<std::endl;
    std::cin>>city;
    strcpy(message.type,city.c_str());

    std::cout<<"INGRESE EL VALOR DE TEMPERATURA"<<std::endl;
    std::cin>>message.newTemperature;
    std::cout<<"INGRESE EL VALOR DE PRESIÓN"<<std::endl;
    std::cin>>message.newPressure;
    std::cout<<"INGRESE EL VALOR DE HUMEDAD"<<std::endl;
    std::cin>>message.newHumidity;

    message.mtype = this->reciverType;
    message.operationType = servicesOperations::SERVICE_OP_UPDATE;
    message.typeService = servicesQuery::SERVICE_WEATHER;

    log(ADMINISTRATOR_NAME + " :Se actualizará el clima en la ciudad "+city,INFORMATION);
    this->mQueue->write(static_cast<const void*>(&message),sizeof(messageAdministrator));
/*    this->mQueue->read(this->mType,static_cast<void*>(&reply),sizeof(messageReplyOperation));

    if(reply.status != typesStatusOperation::ERROR_OPERATION){
        log(ADMINISTRATOR_NAME + " : el clima en la ciudad "+city+" se ha actualizado corretamente",INFORMATION);          
    }else{
        log(ADMINISTRATOR_NAME + " : no se pudo actualizar el clima en la ciudad "+city,INFORMATION);                
    }
*/
}

void Administrator::updateExchangeRate(){
    std::string currency;
    struct messageAdministrator message;
    struct messageReplyOperation reply;

    memset(&message,'\0',sizeof(messageAdministrator));
    memset(&reply,'\0',sizeof(messageReplyOperation));

    std::cout<<"INGRESE EL NOMBRE DE LA MONEDA QUE QUIERE ACTUALIZAR"<<std::endl;
    std::cin.ignore();
    std::getline(std::cin, currency, '\n');
    strcpy(message.type,currency.c_str());
    
    std::cout<<"INGRESE EL VALOR DE TIPO DE CAMBIO"<<std::endl;
    std::cin>>message.newExchangeRate;

    message.mtype = this->reciverType;
    message.operationType = servicesOperations::SERVICE_OP_UPDATE;
    message.typeService = servicesQuery::SERVICE_EXCHANGERATE;    

    log(ADMINISTRATOR_NAME + " : Se actualizará el valor de la moneda "+currency,INFORMATION);
    this->mQueue->write(static_cast<const void*>(&message),sizeof(messageAdministrator));
/*    this->mQueue->read(this->mType,static_cast<void*>(&reply),sizeof(messageReplyOperation));

    if(reply.status != typesStatusOperation::ERROR_OPERATION){
        log(ADMINISTRATOR_NAME + " : el valor de la moneda "+currency+" se ha actualizado corretamente",INFORMATION);    
    }else{
        log(ADMINISTRATOR_NAME + " : no se pudo actualizar el valor de la moneda "+currency,INFORMATION);                
    }
*/
}

void Administrator::deleteWeather(){
    std::string city;
    struct messageAdministrator message;
    struct messageReplyOperation reply;

    memset(&message,'\0',sizeof(messageAdministrator));
    memset(&reply,'\0',sizeof(messageReplyOperation));

    std::cout<<"INGRESE LA CIUDAD QUE QUIERE ELIMINAR DEL REGISTRO DEL CLIMA"<<std::endl;
    std::cin.ignore();
    std::getline(std::cin, city, '\n');
    strcpy(message.type,city.c_str());    

    message.mtype = this->reciverType;
    message.operationType = servicesOperations::SERVICE_OP_ERASE;
    message.typeService = servicesQuery::SERVICE_WEATHER;        

    log(ADMINISTRATOR_NAME + " : Se eliminará del registro del clima la ciudad "+city,INFORMATION);
    this->mQueue->write(static_cast<const void*>(&message),sizeof(messageAdministrator));

/*    this->mQueue->read(this->mType,static_cast<void*>(&reply),sizeof(messageReplyOperation));
    if(reply.status != -1){
        log(ADMINISTRATOR_NAME + " : se ha eliminado del registro del clima la ciudad "+city,INFORMATION);          
    }else{
        log(ADMINISTRATOR_NAME + " : no se pudo eliminar del registro del clima la ciudad "+city,INFORMATION);                
    }
*/
}

void Administrator::deleteExchangeRate(){
    std::string currency;
    struct messageAdministrator message;
    struct messageReplyOperation reply;

    memset(&message,'\0',sizeof(messageAdministrator));
    memset(&reply,'\0',sizeof(messageReplyOperation));

    std::cout<<"INGRESE EL NOMBRE DE LA MONEDA QUE QUIERE ELIMINAR"<<std::endl;
    std::cin.ignore();
    std::getline(std::cin, currency, '\n');
    strcpy(message.type,currency.c_str());    

    message.mtype = this->reciverType;
    message.operationType = servicesOperations::SERVICE_OP_ERASE;
    message.typeService = servicesQuery::SERVICE_EXCHANGERATE;            

    log(ADMINISTRATOR_NAME + " :Se eliminará la moneda "+currency,INFORMATION);
    this->mQueue->write(static_cast<const void*>(&message),sizeof(messageAdministrator));

/*   this->mQueue->read(this->mType,static_cast<void*>(&reply),sizeof(messageReplyOperation));
    if(reply.status != -1){
        log(ADMINISTRATOR_NAME + " : la moneda "+currency+" se ha eliminado corretamente",INFORMATION);    
    }else{
        log(ADMINISTRATOR_NAME + " : no se pudo eliminar la moneda "+currency,INFORMATION);                
    }
*/
}

void Administrator::finalizeServer(){
    struct messageAdministrator message;
    memset(&message,'\0',sizeof(messageAdministrator));
    message.mtype = this->reciverType;
    message.operationType = servicesOperations::SERVICE_OP_END;  
    log(ADMINISTRATOR_NAME + " :Se eliminará el Servidor "+std::to_string(message.mtype),INFORMATION);        
    this->mQueue->write(static_cast<const void*>(&message),sizeof(messageAdministrator));
}

void Administrator::parseMessage(char option){

    switch(option){

        case optionQueryWeather:
            this->solveQueryWeaher();
            break;

        case optionQueryExchangeRate:
            this->solveQueryExchangeRate();
            break;

        case optionUpdateWeather:
            this->updateWeather();
            break;

        case optionUpdateExchangeRate:
            this->updateExchangeRate();
            break;

        case optionDeleteWeather:
            this->deleteWeather();
            break;

        case optionDeleteExchangeRate:
            this->deleteExchangeRate();
            break;

        case optionFinalizeServer:
            this->finalizeServer();
            break;                                                                
            
        case optionDisconnect:
            this->disconnect();
            break;  

        default :
                std::cout<<"opcion ingresada: "<<option<<" incorrecta"<<std::endl;
    }
}

void Administrator::execute(){
       
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

        std::cout<<"--------------------------OPCIONES:-------------------------------"<<std::endl;
        std::cout<<"PARA CONSULTAR EL CLIMA DE UNA CIUDAD INGRESAR LA OPCIÓN: 2"<<std::endl;
        std::cout<<"PARA CONSULTAR EL TIPO DE CAMBIO DE UN PAIS INGRESAR LA OPCIÓN: 3"<<std::endl;
        std::cout<<"------------------------------------------------------------------"<<std::endl;
        std::cout<<"PARA ACTUALIZAR EL CLIMA DE UNA CIUDAD INGRESAR LA OPCIÓN: 4"<<std::endl;
        std::cout<<"PARA ACTUALIZAR EL TIPO DE CAMBIO DE UN PAIS INGRESAR LA OPCIÓN: 5"<<std::endl;
        std::cout<<"------------------------------------------------------------------"<<std::endl;
        std::cout<<"PARA ELIMINAR EL CLIMA DE UNA CIUDAD INGRESAR LA OPCIÓN: 6"<<std::endl;
        std::cout<<"PARA ELIMINAR EL TIPO DE CAMBIO DE UN PAIS INGRESAR LA OPCIÓN: 7"<<std::endl;
        std::cout<<"------------------------------------------------------------------"<<std::endl;
        std::cout<<"PARA FINALIZAR EL SERVIDOR INGRESAR LA OPCIÓN: 9"<<std::endl;
        std::cout<<"------------------------------------------------------------------"<<std::endl;
        std::cout<<"PARA TERMINAR LA CONEXIÓN INGRESAR LA OPCIÓN: 0"<<std::endl;
        std::cout<<"------------------------------------------------------------------"<<std::endl;

        char optionInput = '\0';
        std::cin>>optionInput;
        this->parseMessage(optionInput);
    }
}