#include <string>
#include <vector>
#include "interfaces/Loggable.h"

#ifndef TEAM_H
#define TEAM_H

class Team : public Loggable {

private:
	int firstPlayer; 
	int secondPlayer;
	int setsWon;

public:
	Team(int firstPlayer, int secondPlayer);
	~Team();
	int getFirstPlayer();
	int getSecondPlayer();
	int getSetsWon();
	void setWonSets(int setsWon);
	std::string logMemberVariables();
};

#endif // TEAM_H
