#include <unistd.h>
#include <string>
#include "interfaces/Loggable.h"
#include "Team.h"
#include "tools/utilFunctions.h"
#include "tools/logger.h"
#include "tools/Messages.h"

#ifndef MATCH_H
#define MATCH_H

class Match : public Loggable {

private:
	Team* firstTeam;
	Team* secondTeam ;

public:
	Match(Team* firstTeam, Team* secondTeam);
	~Match();
	void play();
	std::string logMemberVariables();
	messageResult getResultMessage();
};

#endif // MATCH_H
