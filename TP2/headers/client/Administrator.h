#ifndef ADMINISTRATOR_H_
#define ADMINISTRATOR_H_

#include <string>
#include <string.h>
#include <iostream>

#include "tools/Messages.h"
#include "tools/logger.h"
#include "queue/Queue.h"
#include "interfaces/Loggable.h"

#define optionConnect '1'
#define optionReadWeather '2'
#define optionReadExchangeRate '3'
#define optionUpdateWeather '2'
#define optionUpdateExchangeRate '3'
#define optionDeleteWeather '2'
#define optionDeleteExchangeRate '3'
#define optionInsertWeather '2'
#define optionInsertExchangeRate '3'
#define optionDisconnect '0'

class Administrator : public Loggable{

	private:
		long int mType;
		long int reciverType;
		Queue* mQueue;
		bool finalize;
		struct messageReplyWeatherService queryWeather(std::string cityId);
		struct messageReplyExchangeRatesService queryExchangeRate(std::string currencyId);
		void connect();
		void disconnect();
		void updateWeather();
		void updateExchangeRate();
		void deleteWeather();
		void deleteExchangeRate();
		void insertWeather();
		void insertExchangeRate();
        void insertWeather();
		void insertExchangeRate();
		std::string logMemberVariables();
	public:
		Administrator(const std::string& file,const char letter);
		~Administrator();
		void execute();
};

#endif /* ADMINISTRATOR_H_ */