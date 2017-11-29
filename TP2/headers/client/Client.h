#ifndef CLIENT_H_
#define CLIENT_H_

#include <string>
#include <string.h>
#include <iostream>
#include <typeinfo>

#include "ClientAbstract.h"
#include "tools/Messages.h"
#include "tools/logger.h"
#include "queue/Queue.h"
#include "interfaces/Loggable.h"

class Client : public ClientAbstract{
	const std::string CLIENT_NAME = typeid(Client).name();
	private:
		int pid;
	public:
		struct messageReplyWeatherService queryWeather(std::string cityId);
		struct messageReplyExchangeRatesService queryExchangeRate(std::string currencyId);
		Client(const std::string& file,const char letter);
		~Client();
		void execute();
};

#endif /* CLIENTE_H_ */