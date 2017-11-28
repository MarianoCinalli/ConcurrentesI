#ifndef CLIENTABSTRACT_H_
#define CLIENTABSTRACT_H_

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

class ClientAbstract : public Loggable{

	private:
		long int mType;
		long int reciverType;
		Queue* mQueue;
		bool finalize;
		struct messageReplyWeatherService queryWeather(std::string cityId);
		struct messageReplyExchangeRatesService queryExchangeRate(std::string currencyId);
	    enum typesClientConections mTypeClient;
		
	public:
		ClientAbstract(const std::string& file,const char letter, enum typesClientConections typeClient);
		~ClientAbstract();
        void connect();
		void disconnect();
		void solveQueryWeaher();
		void solveQueryExchangeRate();
		std::string logMemberVariables();
		virtual void execute()=0;
        bool getFinalizeProcess(){return this->finalize;}
};

#endif /* CLIENTABSTRACT_H_ */