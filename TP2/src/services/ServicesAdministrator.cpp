#include "services/ServicesAdministrator.h"

void ServicesAdministrator::logIfInvalid(int value, std::string mensaje)
{
    if (value < 0)
    {
        log("**ERROR** " + mensaje, ERROR);
    }
}

ServicesAdministrator::ServicesAdministrator(int queueId)
{
    this->queueId = queueId;
};

ServicesAdministrator::~ServicesAdministrator(){};

void ServicesAdministrator::spawnServices(functionWithIntParametersType weatherService, functionWithIntParametersType exchangeRatesService)
{
    ProcessSpawner *processSpawner = new ProcessSpawner();
    log("ServicesAdministrator: Creando el servicio de cambio de moneda...", INFORMATION);
    int parameters[1] = {this->queueId};
    this->currencyExchangeServicePid = processSpawner->spawnProcess(
        exchangeRatesService,
        parameters);
    log("ServicesAdministrator: Servicio de cambio de moneda ...", INFORMATION);
    this->weatherServicePid = processSpawner->spawnProcess(
        weatherService,
        parameters);
    delete (processSpawner);
}

void ServicesAdministrator::sendReadMessageToCurrencyExchangeService(int replyTo, std::string currency)
{
    messageRequestExchangeRatesService message;
    message.operationType = SERVICE_OP_READ;
    message.mtype = this->currencyExchangeServicePid;
    message.replyTo = replyTo;
    strcpy(message.currency, currency.c_str());
    message.newExchangeRate = -1; // Ignorado
    log("ServicesAdministrator: Enviando mensaje, de lectura de " + currency + ", al servicio de cambio de moneda...", INFORMATION);
    int bytesSend = msgsnd(
        this->queueId,
        static_cast<const void *>(&message),
        sizeof(message) - sizeof(long),
        0);
    this->logIfInvalid(bytesSend, "en el envio del mensaje de lectura.");
    log("ServicesAdministrator: Mensaje de lectura enviado al servicio de cambio de moneda.", INFORMATION);
};

void ServicesAdministrator::sendReadMessageToWeatherService(int replyTo, std::string city)
{
    messageRequestWeatherService message;
    message.operationType = SERVICE_OP_READ;
    message.mtype = this->weatherServicePid;
    message.replyTo = replyTo;
    strcpy(message.city, city.c_str());
    message.newTemperature = -1; // Ignorado
    message.newPressure = -1;    // Ignorado
    message.newHumidity = -1;    // Ignorado
    log("ServicesAdministrator: Enviando mensaje, de lectura de ciudad " + city + ", al servicio de clima...", INFORMATION);
    int bytesSend = msgsnd(
        this->queueId,
        static_cast<const void *>(&message),
        sizeof(message) - sizeof(long),
        0);
    this->logIfInvalid(bytesSend, "en el envio del mensaje de lectura.");
    log("ServicesAdministrator: Mensaje de lectura enviado al servicio de clima.", INFORMATION);
};

void ServicesAdministrator::sendUpdateMessageToCurrencyExchangeService(int replyTo, std::string currency, int newExchangeRate)
{
    messageRequestExchangeRatesService message;
    message.operationType = SERVICE_OP_UPDATE;
    message.mtype = this->currencyExchangeServicePid;
    message.replyTo = replyTo;
    strcpy(message.currency, currency.c_str());
    message.newExchangeRate = newExchangeRate;
    log("ServicesAdministrator: Enviando mensaje, de actualizacion de moneda " + currency + ", al servicio de cambio de moneda...", INFORMATION);
    int bytesSend = msgsnd(
        this->queueId,
        static_cast<const void *>(&message),
        sizeof(message) - sizeof(long),
        0);
    this->logIfInvalid(bytesSend, "en el envio del mensaje de actualizacion.");
    log("ServicesAdministrator: Mensaje de actualizacion enviado al servicio de cambio de moneda.", INFORMATION);
};

void ServicesAdministrator::sendUpdateMessageToWeatherService(int replyTo, std::string city, int newTemperature, int newPressure, int newHumidity)
{
    messageRequestWeatherService message;
    message.operationType = SERVICE_OP_UPDATE;
    message.mtype = this->weatherServicePid;
    message.replyTo = replyTo;
    strcpy(message.city, city.c_str());
    message.newTemperature = newTemperature;
    message.newPressure = newPressure;
    message.newHumidity = newHumidity;
    log("ServicesAdministrator: Enviando mensaje, de actualizacion de ciudad " + city + ", al servicio de clima...", INFORMATION);
    int bytesSend = msgsnd(
        this->queueId,
        static_cast<const void *>(&message),
        sizeof(message) - sizeof(long),
        0);
    this->logIfInvalid(bytesSend, "en el envio del mensaje de actualizacion.");
    log("ServicesAdministrator: Mensaje de actualizacion enviado al servicio de clima.", INFORMATION);
};

void ServicesAdministrator::sendEraseMessageToCurrencyExchangeService(int replyTo, std::string currency)
{
    messageRequestExchangeRatesService message;
    message.operationType = SERVICE_OP_ERASE;
    message.mtype = this->currencyExchangeServicePid;
    message.replyTo = replyTo;
    strcpy(message.currency, currency.c_str());
    message.newExchangeRate = -1; // Ignorado
    log("ServicesAdministrator: Enviando mensaje, de borrado, de moneda " + currency + ", al servicio de cambio de moneda...", INFORMATION);
    int bytesSend = msgsnd(
        this->queueId,
        static_cast<const void *>(&message),
        sizeof(message) - sizeof(long),
        0);
    this->logIfInvalid(bytesSend, "en el envio del mensaje de borado.");
    log("ServicesAdministrator: Mensaje de borrado enviado al servicio de cambio de moneda.", INFORMATION);
};

void ServicesAdministrator::sendEraseMessageToWeatherService(int replyTo, std::string city)
{
    messageRequestWeatherService message;
    message.operationType = SERVICE_OP_ERASE;
    message.mtype = this->weatherServicePid;
    message.replyTo = replyTo;
    strcpy(message.city, city.c_str());
    message.newTemperature = -1; // Ignorado
    message.newPressure = -1;    // Ignorado
    message.newHumidity = -1;    // Ignorado
    log("ServicesAdministrator: Enviando mensaje, de borrado de ciudad " + city + ", al servicio de clima...", INFORMATION);
    int bytesSend = msgsnd(
        this->queueId,
        static_cast<const void *>(&message),
        sizeof(message) - sizeof(long),
        0);
    this->logIfInvalid(bytesSend, "en el envio del mensaje de borado.");
    log("ServicesAdministrator: Mensaje de borrado enviado al servicio de clima.", INFORMATION);
};

// replyTo = getpid() del proceso llamador.
messageReplyWeatherService ServicesAdministrator::recieveMessageFromWeatherService(int replyTo)
{
    messageReplyWeatherService reply;
    log("ServicesAdministrator: Recibiendo mensaje del servicio de clima...", INFORMATION);
    int bytesRecieved = msgrcv(
        this->queueId,
        static_cast<void *>(&reply),
        sizeof(reply) - sizeof(long),
        replyTo,
        0);
    log("ServicesAdministrator: Mensaje recibido.", INFORMATION);
    this->logIfInvalid(bytesRecieved, "en la lectura del mensaje.");
    return reply;
};

// replyTo = getpid() del proceso llamador.
messageReplyExchangeRatesService ServicesAdministrator::recieveMessageFromCurrencyExchangeService(int replyTo)
{
    messageReplyExchangeRatesService reply;
    log("ServicesAdministrator: Recibiendo mensaje del servicio de cambio de monedas...", INFORMATION);
    int bytesRecieved = msgrcv(
        this->queueId,
        static_cast<void *>(&reply),
        sizeof(reply) - sizeof(long),
        replyTo,
        0);
    this->logIfInvalid(bytesRecieved, "en la lectura del mensaje.");
    log("ServicesAdministrator: Mensaje recibido.", INFORMATION);
    return reply;
};

void ServicesAdministrator::endServicesAndReturnWhenFinished()
{
    this->endWeatherServiceAndWait();
    this->endCurrencyExchangeServiceAndWait();
}

void ServicesAdministrator::endWeatherServiceAndWait()
{
    messageRequestWeatherService message;
    message.mtype = this->weatherServicePid;
    message.operationType = SERVICE_OP_END;
    log("ServicesAdministrator: Enviando mensaje de finalizacion al servicio de clima.", INFORMATION);
    msgsnd(
        this->queueId,
        static_cast<const void *>(&message),
        sizeof(message) - sizeof(long),
        0);
    log("ServicesAdministrator: Mensaje enviado.", INFORMATION);
    log("ServicesAdministrator: Esperando al servicio de clima...", INFORMATION);
    int status;
    waitpid(this->weatherServicePid, &status, WUNTRACED);
    log("ServicesAdministrator: Servicio de clima esperado! Status: ", status, INFORMATION);
}

void ServicesAdministrator::endCurrencyExchangeServiceAndWait()
{
    messageRequestExchangeRatesService message;
    message.mtype = this->currencyExchangeServicePid;
    message.operationType = SERVICE_OP_END;
    log("ServicesAdministrator: Enviando mensaje de finalizacion al servicio de cambio de moneda.", INFORMATION);
    msgsnd(
        this->queueId,
        static_cast<const void *>(&message),
        sizeof(message) - sizeof(long),
        0);
    log("ServicesAdministrator: Mensaje enviado.", INFORMATION);
    log("ServicesAdministrator: Esperando al servicio de cambio de moneda...", INFORMATION);
    int status;
    waitpid(this->currencyExchangeServicePid, &status, WUNTRACED);
    log("ServicesAdministrator: Servicio de cambio de moneda esperado! Status: ", status, INFORMATION);
}
int ServicesAdministrator::getWeatherServicePid()
{
    return this->weatherServicePid;
}
int ServicesAdministrator::getCurrencyExchangeServicePid()
{
    return this->currencyExchangeServicePid;
}