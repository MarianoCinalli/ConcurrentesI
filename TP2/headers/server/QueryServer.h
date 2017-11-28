#ifndef QUERYSERVER_H_
#define QUERYSERVER_H_

#include "ServerAbstract.h"
#include <typeinfo>
class QueryServer : ServerAbstract{
	const std::string QUERY_SERVER_NAME = typeid(QueryServer).name();
	private:
        void parseMessage(struct messageQuery message);
	public:
		QueryServer(const std::string& file,const char letter,int clientType);
		~QueryServer();
		void execute();
        
};

#endif /* QUERYSERVER_H_ */