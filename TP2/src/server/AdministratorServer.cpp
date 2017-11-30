#include "server/AdministratorServer.h"
#include "services/ServicesAdministrator.h"
#include "signals/SIGINT_Handler.h"
#include "signals/SignalHandler.h"

extern ServicesAdministrator *ADMINISTRATOR;

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
     // event handler para la senial SIGINT (-2)
    SIGINT_Handler sigint_handler;
 
    // se registra el event handler declarado antes
    SignalHandler :: getInstance()->registrarHandler ( SIGINT,&sigint_handler );
    while (!finalized && sigint_handler.getGracefulQuit() == 0)
    {
        memset(&message, '\0', sizeof(messageAdministrator));
        this->mQueue->read(this->mType, static_cast<void *>(&message), sizeof(messageAdministrator));
        log(ADMINISTRATOR_SERVER_NAME + " :Consulta del administrador con id: ", this->reciverType, INFORMATION);
        this->parseMessage(message);
    }
}

void AdministratorServer::parseMessage(struct messageAdministrator message)
{
    if (message.operationType == servicesOperations::SERVICE_OP_END)
    {   /*
        log(ADMINISTRATOR_SERVER_NAME + " :Apagar a los servidores por parte del Administrador: ", this->reciverType, INFORMATION);
        this->serverIsDied->escribir(true);
        this->finalized = true;
        return;
        */
        log(ADMINISTRATOR_SERVER_NAME + " : Kill a mi parent (SERVER): with pid: ",getppid(),INFORMATION);
        kill(getppid(), SIGINT);
    }

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

    default:
        log(ADMINISTRATOR_SERVER_NAME + " mensaje incorrecto, no se seleccionó ningún tipo de servicio ", WARNING);
    }
}

void AdministratorServer::serviceWeather(struct messageAdministrator message)
{
    switch (message.operationType)
    {

    case servicesOperations::SERVICE_OP_READ:
        struct messageQuery messageQuery;
        memset(&messageQuery, '\0', sizeof(messageQuery));
        strcpy(messageQuery.query, message.type);
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

void AdministratorServer::serviceExchangeRate(struct messageAdministrator message)
{
    switch (message.operationType)
    {

    case servicesOperations::SERVICE_OP_READ:
        struct messageQuery messageQuery;
        memset(&messageQuery, '\0', sizeof(messageQuery));
        strcpy(messageQuery.query, message.type);
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

void AdministratorServer::solveUpdateWeather(struct messageAdministrator message)
{
    std::cout << "Actualizar un registro del servicio del clima" << std::endl;
    std::string city = message.type;
    std::string messageLog = " : datos a actualizar, ciudad: " + city + ", temp: " + std::to_string(message.newTemperature) +
                             ", pres: " + std::to_string(message.newPressure) + ", hum: " + std::to_string(message.newHumidity) +
                             ", idAdmin: ";
    log(ADMINISTRATOR_SERVER_NAME + messageLog, this->reciverType, INFORMATION);

    /*struct messageReplyOperation messageReply;
    messageReply.mtype = this->reciverType;
    messageReply.status = typesStatusOperation::SUCCESS_OPERATION;
    this->mQueue->write(static_cast<const void*>(&messageReply),sizeof(messageReplyOperation));
    */
    ADMINISTRATOR->sendUpdateMessageToWeatherService(this->mType, city,
                                                     message.newTemperature, message.newPressure, message.newHumidity);
}

void AdministratorServer::solveEraseWeather(struct messageAdministrator message)
{
    std::cout << "Eliminar un registro del servicio del clima" << std::endl;
    std::string city = message.type;
    /*struct messageReplyOperation messageReply;
    messageReply.mtype = this->reciverType;
    messageReply.status = typesStatusOperation::SUCCESS_OPERATION;
    this->mQueue->write(static_cast<const void*>(&messageReply),sizeof(messageReplyOperation));
    */
    ADMINISTRATOR->sendEraseMessageToWeatherService(this->mType, city);
}

void AdministratorServer::solveUpdateExchangeRate(struct messageAdministrator message)
{
    std::cout << "Actualizar un registros del servicio de tipo de cambio " << std::endl;
    std::string currency = message.type;
    std::string messageLog = " : datos a actualizar, moneda: " + currency + ", tipoDeCambio: " + std::to_string(message.newExchangeRate) + ", idAdmin: ";
    log(ADMINISTRATOR_SERVER_NAME + messageLog, this->reciverType, INFORMATION);

    /*struct messageReplyOperation messageReply;
    messageReply.mtype = this->reciverType;
    messageReply.status = typesStatusOperation::SUCCESS_OPERATION;
    this->mQueue->write(static_cast<const void*>(&messageReply),sizeof(messageReplyOperation));
    */
    ADMINISTRATOR->sendUpdateMessageToCurrencyExchangeService(this->mType, currency, message.newExchangeRate);
}

void AdministratorServer::solveEraseExchangeRate(struct messageAdministrator message)
{
    std::cout << "Eliminar un registro del servicio de tipo de cambio" << std::endl;
    std::string currency = message.type;
    /*struct messageReplyOperation messageReply;
    messageReply.mtype = this->reciverType;
    messageReply.status = typesStatusOperation::SUCCESS_OPERATION;
    this->mQueue->write(static_cast<const void*>(&messageReply),sizeof(messageReplyOperation)); 
    */
    ADMINISTRATOR->sendEraseMessageToCurrencyExchangeService(this->mType, currency);
}