#include <unistd.h>
#include <string>
#include <vector>
#include "interfaces/Loggable.h"
#include "court/Team.h"
#include "tools/utilFunctions.h"
#include "tools/logger.h"
#include "tools/Messages.h"
#include "tools/Constants.h"

#ifndef MATCH_H
#define MATCH_H

class Match : public Loggable {

private:
	Team* firstTeam;
	Team* secondTeam;
	bool wasCancelled;

public:
	Match(Team* firstTeam, Team* secondTeam);
	~Match();
	void play();
	std::string logMemberVariables();
	messageResult getResultMessage();
	std::vector<messagePlayer> getResultMessages();
	int getMatchStatus();
	messagePlayer getResultMessageForPlayer(int player, int matchStatus);
	void cancelMatch();
	bool wasCancelled();
};

#endif // MATCH_H
