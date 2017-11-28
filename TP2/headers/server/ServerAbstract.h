#ifndef SERVERABSTRACT_H_
#define SERVERABSTRACT_H_

#include <string>

#include "tools/Messages.h"
#include "tools/logger.h"
#include "queue/Queue.h"
#include "interfaces/Loggable.h"

class ServerAbstract : public Loggable{
	protected:
		long int mType;
		long int reciverType;
        bool finalized;
		Queue *mQueue;
        void parseMessage(struct messageQuery message);
		void solveQueryExchangeRate(struct messageQuery message);
        void solveQueryWeather(struct messageQuery message);
        void deleteResources();
	public:
		ServerAbstract(const std::string& file,const char letter,int clientType);
		~ServerAbstract();
		virtual void execute()=0;

    	std::string logMemberVariables();
        
};

#endif /* SERVERABSTRACT_H_ */