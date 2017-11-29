#ifndef CLIENT_H_
#define CLIENT_H_

#include <string>
#include <string.h>
#include <iostream>

#include "ClientAbstract.h"
#include "tools/Messages.h"
#include "tools/logger.h"
#include "queue/Queue.h"
#include "interfaces/Loggable.h"
#include <typeinfo>

class Client : public ClientAbstract{
	const std::string CLIENT_NAME = typeid(Client).name();
	private:

	public:
		Client(const std::string& file,const char letter);
		~Client();
		void execute();
};

#endif /* CLIENTE_H_ */