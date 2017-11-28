#ifndef ADMINISTRATORSERVER_H_
#define ADMINISTRATORSERVER_H_

#include "ServerAbstract.h"

class AdministratorServer : public ServerAbstract{
	private:
		void parseMessage(struct messageQuery message);
		/*
			get, insert, update, delete from ExchangeRate and Wheather
		*/
		void solveQueryExchangeRate(struct messageQuery message);
		void solveQueryWeather(struct messageQuery message);
	public:
		AdministratorServer(const std::string& file,const char letter,int clientType);
		~AdministratorServer();
		void execute();
    	std::string logMemberVariables();
        
};

#endif /* ADMINISTRATORSERVER_H_ */