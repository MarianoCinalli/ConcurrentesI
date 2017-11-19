#ifndef MESSAGES_H_
#define MESSAGES_H_

#include "Constants.h"

	struct messageRequestWeatherService {
		long mtype;
		long replyTo;
		int cityId;
	};

	struct messageRequestExchangeRatesService {
		long mtype;
		long replyTo;
		int currencyId;
	};

	// [Temperature] = Celsius - [Pressure] = ATM  - [Humidity] = %
	struct messageReplyWeatherService {
		long mtype;
		int temperature;
		int pressure;
		int humidity;
		int errorId;
	};

	// 1 ARG Peso = exchangeRate currency
	struct messageReplyExchangeRatesService {
		long mtype;
		int exchangeRate;
		int errorId;
	};

#endif // MESSAGES_H_
