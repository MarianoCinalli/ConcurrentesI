#ifndef ADMINISTRATOR_H_
#define ADMINISTRATOR_H_

#include <string>
#include <string.h>
#include <iostream>

#include "tools/Messages.h"
#include "tools/logger.h"
#include "queue/Queue.h"
#include "ClientAbstract.h"

#define optionUpdateWeather '4'
#define optionUpdateExchangeRate '5'
#define optionDeleteWeather '6'
#define optionDeleteExchangeRate '7'
#define optionFinalizeServer '9'

class Administrator : public ClientAbstract{
	const std::string ADMINISTRATOR_NAME = typeid(Administrator).name();
	private:
		void updateWeather();
		void updateExchangeRate();
		void deleteWeather();
		void deleteExchangeRate();
		void parseMessage(char option);
		void finalizeServer();
	public:
		Administrator(const std::string& file,const char letter);
		~Administrator();
		void execute();
};

#endif /* ADMINISTRATOR_H_ */