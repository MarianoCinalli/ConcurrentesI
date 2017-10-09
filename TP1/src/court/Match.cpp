#include "court/Match.h"

Match::Match(Team* firstTeam, Team* secondTeam) {
	this->firstTeam = firstTeam;
	this->secondTeam = secondTeam;
};

Match::~Match() {
	delete(this->firstTeam);
	delete(this->secondTeam);
};

void Match::play() {
	int matchLength = getRandomBetween(1, 5);
	sleep(matchLength);
	log("El partido duro: ", matchLength, 3);
	int firstTeamWonSets = getRandomBetween(1, 5);
	this->firstTeam->setWonSets(firstTeamWonSets);
	this->secondTeam->setWonSets(5 - firstTeamWonSets);
}

std::string Match::logMemberVariables() {
	return "\nPrimer equipo: " + this->firstTeam->logMemberVariables()
	+ "\nSegundo equipo: " + this->secondTeam->logMemberVariables();
};

messageResult Match::getResultMessage() {
	struct messageResult message;
	message.idPlayer1_Team1 = this->firstTeam->getFirstPlayer();
	message.idPlayer2_team1 = this->firstTeam->getSecondPlayer();
	message.setsWonTeam1 = this->firstTeam->getSetsWon();
	message.idPlayer1_Team2 = this->secondTeam->getFirstPlayer();
	message.idPlayer2_team2 = this->secondTeam->getSecondPlayer();
	message.setsWonTeam2 = this->secondTeam->getSetsWon();
	return message;
}
