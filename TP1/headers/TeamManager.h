
/*
* TeamManager: Read from FifoTeamManager and tries to build team.
* If it build a team, it will write a team message in FifoMatchManager
*
*/
#ifndef TEAMMANAGER_H
#define TEAMMANAGER_H

#include <string.h>
#include <map>
#include <vector>
#include <algorithm>    // std::find
#include "tools/Constants.h"
#include "tools/Messages.h"
#include "interfaces/Executable.h"
#include "fifos/FifoLectura.h"
#include "fifos/FifoEscritura.h"
#include "tools/logger.h"
#include <typeinfo>

class TeamManager{//: public Executable {

    const std::string TEAM_MANAGER_NAME = typeid(TeamManager).name();

    private:
        
        bool finalize = false;
        std::map<int, std::vector<int>*> *playsByPlayer;
        std::vector<int> *players;
        FifoLectura *channelToRead;
        FifoEscritura *channelToWrite;
        void cancelLastGameOfPLayer(int idPlayer);
        void addPlayer(int idPlayer);
        struct messageTeam* makeTeam();
        struct messagePlayer* readPlayer();
        void writeTeam(struct messageTeam* team);
        void parseMessage(struct messagePlayer* message);
        bool canPlayersFormTeam(int idPlayer1, int idPlayer2);
        struct messageTeam* formTeam(int idPlayer1, int idPlayer2);
        void removePlayer(int idPlayer);

    public:

        TeamManager();
        ~TeamManager();
        void execute();

};

#endif // TEAMMANAGER_H
