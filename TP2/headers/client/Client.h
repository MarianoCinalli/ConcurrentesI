#ifndef CLIENT_H_
#define CLIENT_H_

#include <string>
#include <string.h>
#include <iostream>

#include "tools/Messages.h"
#include "tools/logger.h"
#include "queue/Queue.h"
#include "interfaces/Loggable.h"

#define optionConnect '1'
#define optionQueryWeather '2'
#define optionQueryExchangeRate '3'
#define optionDisconnect '0'

class Client : public Loggable{

	private:
		long int mType;
		long int reciverType;
		Queue* mQueue;
		bool finalize;
		struct messageReplyWeatherService queryWeather(std::string cityId);
		struct messageReplyExchangeRatesService queryExchangeRate(std::string currencyId);
		void connect();
		void disconnect();
		void solveQueryWeaher();
		void solveQueryExchangeRate();
		std::string logMemberVariables();
	public:
		Client(const std::string& file,const char letter);
		~Client();
		void execute();
};

#endif /* CLIENTE_H_ */