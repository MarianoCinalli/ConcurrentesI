#ifndef ADMINISTRATORSERVER_H_
#define ADMINISTRATORSERVER_H_

#include "ServerAbstract.h"
#include <typeinfo>

class AdministratorServer : public ServerAbstract{
	const std::string ADMINISTRATOR_SERVER_NAME = typeid(AdministratorServer).name();
	private:
		void parseMessage(struct messageAdministrator message);
		/*
			get, insert, update, delete from ExchangeRate and Wheather
		*/
		void serviceWeather(struct messageAdministrator message);
		void serviceExchangeRate(struct messageAdministrator message);

		void solveUpdateWeather(struct messageAdministrator message);
		void solveEraseWeather(struct messageAdministrator message);
		
		void solveUpdateExchangeRate(struct messageAdministrator message);
		void solveEraseExchangeRate(struct messageAdministrator message);
	public:
		AdministratorServer(const std::string& file,const char letter,int clientType);
		~AdministratorServer();
		void execute();        
};

#endif /* ADMINISTRATORSERVER_H_ */