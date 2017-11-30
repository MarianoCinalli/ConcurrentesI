#ifndef SERVICESADMINISTRATOR_H
#define SERVICESADMINISTRATOR_H

#include <string>
#include <string.h>

#include "tools/logger.h"
#include "tools/Messages.h"
#include "tools/Constants.h"
#include "tools/ProcessSpawner.h"

// To send message to the queue.
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

typedef void (*functionWithIntParametersType)(int parameters[]);

class ServicesAdministrator {

	private:
		int queueId;
		int weatherServicePid;
		int currencyExchangeServicePid;

	public:
		int getWeatherServicePid();
		int getCurrencyExchangeServicePid();
		ServicesAdministrator(int queueId);
		~ServicesAdministrator();
		void spawnServices(functionWithIntParametersType weatherService, functionWithIntParametersType exchangeRatesService);
		void sendReadMessageToCurrencyExchangeService(int replyTo, std::string currency);
		void sendReadMessageToWeatherService(int replyTo, std::string city);
		void sendUpdateMessageToCurrencyExchangeService(int replyTo, std::string currency, int newExchangeRate);
		void sendUpdateMessageToWeatherService(int replyTo, std::string city, int newTemperature, int newPressure, int newHumidity);
		void sendEraseMessageToCurrencyExchangeService(int replyTo, std::string currency);
		void sendEraseMessageToWeatherService(int replyTo, std::string city);
		messageReplyWeatherService recieveMessageFromWeatherService(int replyTo);
		messageReplyExchangeRatesService recieveMessageFromCurrencyExchangeService(int replyTo);
		void endServicesAndReturnWhenFinished();

	private:
		void endWeatherServiceAndWait();
		void endCurrencyExchangeServiceAndWait();
		void logIfInvalid(int value, std::string mensaje);
};

#endif // SERVICESADMINISTRATOR_H
