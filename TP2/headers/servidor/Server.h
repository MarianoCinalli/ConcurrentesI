#ifndef SERVER_H_
#define SERVER_H_

#include <string>
#include <string.h>

#include "tools/Messages.h"
#include "../cola/Queue.h"
#include "../interfaces/Loggable.h"

class Server : public Loggable{

	private:
		long int mtype;
		long int recivertype;

	public:
		Server ( const std::string& archivo,const char letra );
		~Server();
		void execute();
        
};

#endif /* SERVER_H_ */