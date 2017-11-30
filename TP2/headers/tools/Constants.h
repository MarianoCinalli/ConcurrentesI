#ifndef CONSTANTS_H_
#define CONSTANTS_H_

const std::string WEATHER_DB_FILE = "db/weather.json";
const std::string EXCHANGE_RATES_DB_FILE = "db/exchangeRates.json";
static const std::string SHM = "/opt/memoria.txt";
static const char LETRA = 'a';

enum logLevels {
	ERROR = 1,
	WARNING = 2,
	INFORMATION = 3
};

// Para los errores en las operaciones de los servicios.
enum servicesErrorsIds {
	NO_ERROR = 0,
	NOT_FOUND = 1
};

// Tipos de operaciones soportadas por servicios.
enum servicesOperations {
	SERVICE_OP_ERASE = 0,
	SERVICE_OP_UPDATE = 1,
	SERVICE_OP_READ = 2,
	SERVICE_OP_END = 3
};

// Tipos de consulta de un cliente a un servidor.
enum servicesQuery {
	SERVICE_WEATHER = 0,
	SERVICE_EXCHANGERATE = 1,
	END_CONECTION = 2,
};

enum typesClientConections{
	CLIENT_CONECTION = 0,
	ADMINISTRATOR_CONECTION =1
};

enum typesStatusOperation{
	ERROR_OPERATION = -1,
	SUCCESS_OPERATION =0
};

#endif // CONSTANTS_H_
