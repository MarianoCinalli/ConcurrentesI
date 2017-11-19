#ifndef CLIENT_H_
#define CLIENT_H_

#include <string>
#include <string.h>

#include "tools/Messages.h"
#include "../cola/Queue.h"
#include "../interfaces/Loggable.h"

class Client : public Loggable{

	private:
		long int mtype;
		long int recivertype;
	public:
		Client ( const std::string& archivo,const char letra );
		~Client();
		void connect();
		void disconnet();
		void queryWeather();
		void queryExchange();
};

#endif /* CLIENTE_H_ */