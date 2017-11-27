#ifndef CLIENT_H_
#define CLIENT_H_

#include <string>
#include <string.h>
#include <iostream>

#include "tools/Messages.h"
#include "tools/logger.h"
#include "queue/Queue.h"
#include "interfaces/Loggable.h"

#define optionConnect 1
#define optionQueryWeather 2
#define optionQueryExchangeRate 3
#define optionDisconnect 0


class Client : public Loggable{

	private:
		long int mType;
		long int reciverType;
		Queue* mQueue;
		bool finalize;
		void execute();
	public:
		Client(const std::string& file,const char letter);
		~Client();
		void connect();
		void disconnect();
		void solveQueryWeaher();
		void solveQueryExchangeRate();
		struct messageReplyWeatherService queryWeather(std::string cityId);
		struct messageReplyExchangeRatesService queryExchangeRate(std::string currencyId);
		std::string logMemberVariables();
};

#endif /* CLIENTE_H_ */