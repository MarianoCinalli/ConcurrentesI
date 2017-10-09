
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

const std::string FILE_FIFO_READ = "/tmp/file_fifo_PlayerManager_TeamManager";
const std::string FILE_FIFO_WRITE = "/tmp/file_fifo_TeamManager_MatchManager";

class TeamManager{//: public Executable {

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
        bool playersPlayBetween(int idPlayer1, int idPlayer2);

    public:

        TeamManager();
        ~TeamManager();
        void execute();

};

#endif // TEAMMANAGER_H
