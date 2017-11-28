#ifndef QUERYSERVER_H_
#define QUERYSERVER_H_

#include "ServerAbstract.h"

class QueryServer : ServerAbstract{
	private:
        void parseMessage(struct messageQuery message);
		void solveQueryExchangeRate(struct messageQuery message);
		void solveQueryWeather(struct messageQuery message);
	public:
		QueryServer(const std::string& file,const char letter,int clientType);
		~QueryServer();
		void execute();
    	std::string logMemberVariables();
        
};

#endif /* QUERYSERVER_H_ */