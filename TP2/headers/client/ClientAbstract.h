#ifndef CLIENTABSTRACT_H_
#define CLIENTABSTRACT_H_

#include <string>
#include <string.h>
#include <iostream>
#include <typeinfo>

#include "tools/Messages.h"
#include "tools/logger.h"
#include "queue/Queue.h"
#include "interfaces/Loggable.h"

#define optionConnect '1'
#define optionQueryWeather '2'
#define optionQueryExchangeRate '3'
#define optionDisconnect '0'

class ClientAbstract : public Loggable{
	
	const std::string CLIENT_ABSTRACT_NAME = typeid(ClientAbstract).name();
	protected:
		long int mType;
		long int reciverType;
		Queue* mQueue;
		bool finalized;

	private:
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
		virtual struct messageReplyExchangeRatesService queryExchangeRate(std::string currency)=0;
		virtual struct messageReplyWeatherService queryWeather(std::string city)=0;
        bool getFinalizeProcess(){return this->finalized;}
};

#endif /* CLIENTABSTRACT_H_ */