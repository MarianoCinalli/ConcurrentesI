#ifndef CONSTANTS_H_
#define CONSTANTS_H_

const std::string WEATHER_DB_FILE = "db/weather.json";
const std::string EXCHANGE_RATES_DB_FILE = "db/exchangeRates.json";

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

#endif // CONSTANTS_H_
