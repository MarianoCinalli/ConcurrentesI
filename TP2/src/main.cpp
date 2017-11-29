#include <iostream>
#include <string>
#include "tools/logger.h"
#include "tools/ProcessSpawner.h"

// Services
#include "services/ExchangeRatesService.h"
#include "services/WeatherService.h"

// Queues
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// Constants ------------------------------------------------------
int LOG_MIN_LEVEL = 1;
std::ofstream LOG_FILE_POINTER;
// Functions ------------------------------------------------------
void executeExchangeRatesService(int parameters[]){
    log("main: Inicio del servicion de cambio de moneda.",INFORMATION);
    ExchangeRatesService* service = new ExchangeRatesService(parameters[0]);
    service->run();
    delete service;
    log("main: Fin del servicio de cambio de moneda.",INFORMATION);
}

void executeWeatherService(int parameters[]){
    log("main: Inicio del servicion de clima.",INFORMATION);
    WeatherService* service = new WeatherService(parameters[0]);
    service->run();
    delete service;
    log("main: Fin del servicio de clima.",INFORMATION);
}

typedef void (*functiontype)();

int main(int argc, char* argv[]) {
    // Initialization
    srand(time(NULL)); // Init seed for random
    LOG_FILE_POINTER.open("tp2.log", std::ofstream::app);
    // End Initialization
    logSessionStarted();

	logSessionFinished();
	return 0;
}
