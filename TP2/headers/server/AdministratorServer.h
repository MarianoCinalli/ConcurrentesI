#ifndef ADMINISTRATORSERVER_H_
#define ADMINISTRATORSERVER_H_

#include "ServerAbstract.h"
#include <typeinfo>

class AdministratorServer : public ServerAbstract{
	const std::string ADMINISTRATOR_SERVER_NAME = typeid(AdministratorServer).name();
	private:
		void parseMessage(struct messageQuery message);
		/*
			get, insert, update, delete from ExchangeRate and Wheather
		*/
	public:
		AdministratorServer(const std::string& file,const char letter,int clientType);
		~AdministratorServer();
		void execute();        
};

#endif /* ADMINISTRATORSERVER_H_ */