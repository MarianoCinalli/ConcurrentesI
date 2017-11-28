#ifndef QUERYSERVER_H_
#define QUERYSERVER_H_

#include <string>

#include "tools/Messages.h"
#include "tools/logger.h"
#include "queue/Queue.h"
#include "interfaces/Loggable.h"
#include "server/ServerAbstract.h"

class QueryServer : public ServerAbstract{
	private:
		/*long int mType;
		long int reciverType;
        bool finalized;
		Queue *mQueue;
		void solveQueryExchangeRate(struct messageQuery message);
		void solveQueryWeather(struct messageQuery message);
		*/void parseMessage(struct messageQuery message);
	public:
		QueryServer(const std::string& file,const char letter,int clientType);
		~QueryServer();
		void execute();
    	//std::string logMemberVariables();
        
};

#endif /* QUERYSERVER_H_ */