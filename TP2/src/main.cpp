#include <iostream>
#include <string>
#include "tools/logger.h"

// Services
#include "services/ExchangeRatesService.h"
#include "services/WeatherService.h"
#include "services/ServicesAdministrator.h"

// Constants ------------------------------------------------------
int LOG_MIN_LEVEL = 1;
std::ofstream LOG_FILE_POINTER;
// Spawn Services -------------------------------------------------
void executeExchangeRatesService(int parameters[]){
    log("main: Inicio del servicio de cambio de moneda.", INFORMATION);
    ExchangeRatesService* service = new ExchangeRatesService(parameters[0]);
    service->run();
    delete service;
    log("main: Fin del servicio de cambio de moneda.", INFORMATION);
};

void executeWeatherService(int parameters[]){
    log("main: Inicio del servicio de clima.", INFORMATION);
    WeatherService* service = new WeatherService(parameters[0]);
    service->run();
    delete service;
    log("main: Fin del servicio de clima.", INFORMATION);
};
// End Spawn Services ---------------------------------------------

int main(int argc, char* argv[]) {
    // Initialization
    LOG_FILE_POINTER.open("tp2.log", std::ofstream::app);
    // End Initialization
    logSessionStarted();

    // -----------------------------------------------------
    key_t clave = ftok("tp2.log", 'A');
    int queueId = msgget(clave, 0777 | IPC_CREAT);

    ServicesAdministrator* administrator = new ServicesAdministrator(queueId);
    administrator->spawnServices(&executeWeatherService, &executeExchangeRatesService);

    int thisPocessPid = getpid();

    // READ AND REPLY EXAMPLES
    administrator->sendReadMessageToWeatherService(thisPocessPid, "buenos aires");

    messageReplyWeatherService weatherReply = administrator->recieveMessageFromWeatherService(thisPocessPid);
    if (weatherReply.errorId == NO_ERROR) {
        std::cout << "Temperatura: " << weatherReply.temperature <<
            " - Presion:" << weatherReply.pressure <<
            " - Humedad:" << weatherReply.humidity << std::endl;
    } else {
        std::cout << "Error" << std::endl;
    }

    administrator->sendReadMessageToCurrencyExchangeService(thisPocessPid, "dolar");

    messageReplyExchangeRatesService currencyReply = administrator->recieveMessageFromCurrencyExchangeService(thisPocessPid);
    if (currencyReply.errorId == NO_ERROR) {
        std::cout << "1 peso = " << currencyReply.exchangeRate << " dolares" << std::endl;
    } else {
        std::cout << "Error" << std::endl;
    }

    // UPDATE EXAMPLES
    administrator->sendUpdateMessageToCurrencyExchangeService(thisPocessPid, "peso uruguayo", 4); // Actualizar
    administrator->sendUpdateMessageToWeatherService(thisPocessPid, "ciudadInexistente", 23, 1, 100); // Crear porque no existe

    // ERASE EXAMPLES
    administrator->sendEraseMessageToCurrencyExchangeService(thisPocessPid, "euro");
    administrator->sendEraseMessageToWeatherService(thisPocessPid, "maipu");

    // END SERVICES EXAMPLE
    administrator->endServicesAndReturnWhenFinished();

    delete(administrator);
    // -----------------------------------------------------

    msgctl(queueId, IPC_RMID, NULL);
    logSessionFinished();
    return 0;
}
