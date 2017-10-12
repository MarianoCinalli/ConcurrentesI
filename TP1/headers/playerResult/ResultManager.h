#ifndef RESULTMANAGER_H
#define RESULTMANAGER_H

#include "../fifos/FifoLectura.h"
#include <map>
#include "PlayerResult.h"
#include <string.h>
#include "../tools/Constants.h"
#include "../tools/Messages.h"
#include "../tools/logger.h"
#include <typeinfo>

class ResultManager {

    const std::string RESULT_MANAGER_NAME = typeid(ResultManager).name();
    const int THREE = 3;
    const int TWO = 2;
    const int ONE = 1;
    const int ZERO = 0;

private:

		bool finalizedProcess;
		FifoLectura *channelToRead;
		std::map<int, PlayerResult*> *playerResults;

        struct messageResult* readFifoResultManager();
        PlayerResult* searchPlayerResult(int idPlayer);

	public:
		ResultManager();
		~ResultManager();
		void execute();
};

#endif // RESULTMANAGER_H
