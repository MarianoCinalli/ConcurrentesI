
/*
* MatchManager: Read from FifoMatchManager and tries to build team.
* If it builds a team, it will write a team message in FifoMatches
*
*/
#ifndef MATCHMANAGER_H
#define MATCHMANAGER_H

#include <string.h>
#include "tools/Constants.h"
#include "tools/Messages.h"
#include "interfaces/Executable.h"
#include "fifos/FifoLectura.h"
#include "fifos/FifoEscritura.h"
#include "tools/logger.h"
#include <typeinfo>

class MatchManager : public Executable {

    const std::string MATCH_MANAGER_NAME = typeid(MatchManager).name();

    private:
        
        bool finalize = false;
        FifoLectura *channelToReadTeams;
        FifoEscritura *channelToWriteMatches;
        messageMatch* makeMatch(struct messageTeam* team1, struct messageTeam* team2);
        void writeMatch(struct messageMatch *match);
        messageTeam* readTeam();
        struct messageTeam* team1;
        struct messageTeam* team2;

        void parseMessage(struct messageTeam* team);
        void notifyMatch(struct messageTeam* team);
        void notifyCloseMatches();

    public:

        MatchManager();
        ~MatchManager();
        void execute();

};

#endif // MATCHMANAGER_H
