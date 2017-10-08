
/*
* TeamManager: Read from FifoTeamManager and tries to build team.
* If it build a team, it will write a team message in FifoMatchManager
*
*/
#ifndef TEAMMANAGER_H
#define TEAMMANAGER_H

#include <string>
#include <map>
#include <vector>
#include <algorithm>    // std::find
#include "tools/Constants.h"
#include "interfaces/Executable.h"

const std::string FILE_FIFO_PLAYER = "/tmp/file_fifo_PlayerManager_TeamManager";
const std::string FILE_FIFO_TEAM = "/tmp/file_fifo_TeamManager_MatchManager";

class TeamManager: public Executable {

    private:
        
        bool finalize = false;
        std::map<int, std::vector<int>*> *playsByPlayer;


    public:

        TeamManager();
        ~TeamManager();
        bool playersPlayBetween(int idPlayer1, int idPlayer2);
        void execute();

};

#endif // TEAMMANAGER_H
