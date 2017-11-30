#include "server/QueryServer.h"
#include "signals/SIGINT_Handler.h"
#include "signals/SignalHandler.h"

QueryServer::QueryServer(const std::string &file, const char letter, int clientType)
    : ServerAbstract(file, letter, clientType)
{
    log(QUERY_SERVER_NAME + " :Inicializador para atender a cliente con id: ", clientType, INFORMATION);
}

QueryServer::~QueryServer()
{
}

void QueryServer::execute()
{
    //aceptando la conexión
    struct messageConection conection;
    conection.mtype = this->reciverType;
    conection.senderType = this->mType;
    this->mQueue->write(static_cast<const void *>(&conection), sizeof(messageConection));
    log(QUERY_SERVER_NAME + " :Execute loop para atender a cliente con id: ", this->mType, INFORMATION);

    struct messageQuery message;
    // event handler para la senial SIGINT (-2)
    SIGINT_Handler sigint_handler;
 
    // se registra el event handler declarado antes
    SignalHandler :: getInstance()->registrarHandler ( SIGINT,&sigint_handler );
    while (!finalized && sigint_handler.getGracefulQuit() == 0)
        {
            memset(&message, '\0', sizeof(messageQuery));
            this->mQueue->read(this->mType, static_cast<void *>(&message), sizeof(messageQuery));
            log(QUERY_SERVER_NAME + " :Consulta del cliente con id: ", this->reciverType, INFORMATION);
            this->parseMessage(message);
        }
}

void QueryServer::parseMessage(struct messageQuery message)
{

    switch (message.queryType)
    {

    case servicesQuery::SERVICE_WEATHER:
        this->solveQueryWeather(message);
        break;

    case servicesQuery::SERVICE_EXCHANGERATE:
        this->solveQueryExchangeRate(message);
        break;

    case servicesQuery::END_CONECTION:
        log(QUERY_SERVER_NAME + " :Se recibe el mensaje de finalización de conexión, cliente con id: ", this->reciverType, INFORMATION);
        std::cout << "Fin de la conexión" << std::endl;
        this->finalized = true;
        break;
    }
}