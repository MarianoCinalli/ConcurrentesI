#ifndef SERVER_H_
#define SERVER_H_

#include <string>
#include <string.h>

#include "tools/Messages.h"
#include "tools/logger.h"
#include "queue/Queue.h"
#include "interfaces/Loggable.h"
#include "QueryServer.h"
#include "AdministratorServer.h"
#include <typeinfo>

class Server : public Loggable{
	const std::string SERVER_NAME = typeid(Server).name();
	private:
		long int mType;
		long int reciverType;
		Queue *mQueue;
		std::string file;
		int amountQueryServers;
    	int maxQueryServer;
		char letter;
		bool finalized;
		void createQueryServers();
        void assingQueryServer(int clientType);
        void createQueryServer(struct messageConection connection);
	public:
		Server(const std::string& file,const char letter,int amountQueryServers,int maxQueryServer);
		~Server();
		void execute();
		std::string logMemberVariables();
        
};

#endif /* SERVER_H_ */