#include "server/AdministratorServer.h"

AdministratorServer::AdministratorServer(const std::string &file, const char letter, int clientType)
    : ServerAbstract(file, letter, clientType)
{
    log(ADMINISTRATOR_SERVER_NAME + " :Inicializador para atender a cliente con id: ", clientType, INFORMATION);
}

AdministratorServer::~AdministratorServer()
{
}

void AdministratorServer::execute()
{
    //aceptando la conexión
    struct messageConection conection;
    conection.mtype = this->reciverType;
    conection.senderType = this->mType;
    this->mQueue->write(static_cast<const void *>(&conection), sizeof(messageConection));
    log(ADMINISTRATOR_SERVER_NAME + " :Execute loop para atender a cliente con id: ", this->mType, INFORMATION);

    struct messageAdministrator message;
    while (!finalized)
    {
        memset(&message, '\0', sizeof(messageAdministrator));
        this->mQueue->read(this->mType, static_cast<void *>(&message), sizeof(messageAdministrator));
        log(ADMINISTRATOR_SERVER_NAME + " :Consulta del administrador con id: ", this->reciverType, INFORMATION);
        this->parseMessage(message);
    }
}

void AdministratorServer::parseMessage(struct messageAdministrator message)
{

    switch (message.typeService)
    {

    case servicesQuery::SERVICE_WEATHER:
        this->serviceWeather(message);
        break;

    case servicesQuery::SERVICE_EXCHANGERATE:
        this->serviceExchangeRate(message);
        break;

    case servicesQuery::END_CONECTION:
        log(ADMINISTRATOR_SERVER_NAME + " :Se recibe el mensaje de finalización de conexión, cliente con id: ", this->reciverType, INFORMATION);
        std::cout << "Fin de la conexión" << std::endl;
        this->finalized = true;
        break;
    
    default :
        log(ADMINISTRATOR_SERVER_NAME + " mensaje incorrecto, no se seleccionó ningún tipo de servicio ", WARNING);        
    }
}

void AdministratorServer::serviceWeather(struct messageAdministrator message){
    switch (message.operationType){

    case servicesOperations::SERVICE_OP_READ:
        struct messageQuery messageQuery;
        memset(&messageQuery,'\0',sizeof(messageQuery));
        strcpy(messageQuery.query,message.type);
        this->solveQueryWeather(messageQuery);
        break;

    case servicesOperations::SERVICE_OP_UPDATE:
        this->solveUpdateWeather(message);
        break;

    case servicesOperations::SERVICE_OP_ERASE:
        this->solveEraseWeather(message);
        break;
    }
}

void AdministratorServer::serviceExchangeRate(struct messageAdministrator message){
    switch (message.operationType){

    case servicesOperations::SERVICE_OP_READ:
        struct messageQuery messageQuery;
        memset(&messageQuery,'\0',sizeof(messageQuery));
        strcpy(messageQuery.query,message.type);
        this->solveQueryWeather(messageQuery);
        break;

    case servicesOperations::SERVICE_OP_UPDATE:
        this->solveUpdateExchangeRate(message);
        break;

    case servicesOperations::SERVICE_OP_ERASE:
        this->solveEraseExchangeRate(message);
        break;
    }
}

void AdministratorServer::solveUpdateWeather(struct messageAdministrator message){
    std::cout<<"Actualizar un registro del servicio del clima"<<std::endl;
    /*struct messageReplyOperation messageReply;
    messageReply.mtype = this->reciverType;
    messageReply.status = typesStatusOperation::SUCCESS_OPERATION;
    this->mQueue->write(static_cast<const void*>(&messageReply),sizeof(messageReplyOperation));
    */
}

void AdministratorServer::solveEraseWeather(struct messageAdministrator message){
    std::cout<<"Eliminar un registro del servicio del clima"<<std::endl;
    /*struct messageReplyOperation messageReply;
    messageReply.mtype = this->reciverType;
    messageReply.status = typesStatusOperation::SUCCESS_OPERATION;
    this->mQueue->write(static_cast<const void*>(&messageReply),sizeof(messageReplyOperation));
    */
}

void AdministratorServer::solveUpdateExchangeRate(struct messageAdministrator message){
    std::cout<<"Actualizar un registros del servicio de tipo de cambio "<<std::endl;
    /*struct messageReplyOperation messageReply;
    messageReply.mtype = this->reciverType;
    messageReply.status = typesStatusOperation::SUCCESS_OPERATION;
    this->mQueue->write(static_cast<const void*>(&messageReply),sizeof(messageReplyOperation));
    */
}

void AdministratorServer::solveEraseExchangeRate(struct messageAdministrator message){
    std::cout<<"Eliminar un registro del servicio de tipo de cambio"<<std::endl;
    /*struct messageReplyOperation messageReply;
    messageReply.mtype = this->reciverType;
    messageReply.status = typesStatusOperation::SUCCESS_OPERATION;
    this->mQueue->write(static_cast<const void*>(&messageReply),sizeof(messageReplyOperation)); 
    */  
}