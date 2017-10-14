#ifndef RESULTMANAGER_H
#define RESULTMANAGER_H

#include "../fifos/FifoLectura.h"
#include <map>
#include <vector>
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
        int maxScore;
        std::vector<PlayerResult*> *winners;

        struct messageResult* readFifoResultManager();
        PlayerResult* searchPlayerResult(int idPlayer);
        void managerWinners(PlayerResult *player1Team1Result, PlayerResult *player2Team1Result, PlayerResult *player1Team2Result, PlayerResult *player2Team2Result);
        void managerWinner(PlayerResult *playerResult);
        void proccesResult(struct messageResult *result);
        void parseMessage(struct messageResult *result);

	public:
		ResultManager();
		~ResultManager();
        void execute();
        void showWinners();
};

#endif // RESULTMANAGER_H
