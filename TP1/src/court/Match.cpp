#include "court/Match.h"

Match::Match(Team* firstTeam, Team* secondTeam) {
	this->firstTeam = firstTeam;
	this->secondTeam = secondTeam;
	this->wasCancelled = false;
	this->matchFinished = false;
};

Match::~Match() {
	delete(this->firstTeam);
	delete(this->secondTeam);
};

void Match::play() {
	int matchLength = getRandomBetween(1, 5);
	sleep(matchLength);
	log("El partido duro: ", matchLength, 3);
	int firstTeamWonSets = getRandomBetween(0, 3);
	this->firstTeam->setWonSets(firstTeamWonSets);
	if (firstTeamWonSets < 3) {
		this->secondTeam->setWonSets(3);
	} else {
		this->secondTeam->setWonSets(5 - firstTeamWonSets);
	}
	this->matchFinished = true;
}

std::string Match::logMemberVariables() {
	if (this->wasCancelled) {
		return "\nPARTIDO CANCELADO! \nPrimer equipo: " + this->firstTeam->logMemberVariables()
		+ "\nSegundo equipo: " + this->secondTeam->logMemberVariables();
	} else {
		return "\nPrimer equipo: " + this->firstTeam->logMemberVariables()
		+ "\nSegundo equipo: " + this->secondTeam->logMemberVariables();	
	}

};

messageResult Match::getResultMessage() {
	struct messageResult message;
	message.operation = ResultCommands::RESULT;
	message.idPlayer1_Team1 = this->firstTeam->getFirstPlayer();
	message.idPlayer2_team1 = this->firstTeam->getSecondPlayer();
	message.setsWonTeam1 = this->firstTeam->getSetsWon();
	message.idPlayer1_Team2 = this->secondTeam->getFirstPlayer();
	message.idPlayer2_team2 = this->secondTeam->getSecondPlayer();
	message.setsWonTeam2 = this->secondTeam->getSetsWon();
	return message;
};

std::vector<messagePlayer> Match::getResultMessages() {
	std::vector<messagePlayer> messages;
	int matchStatus = this->getMatchStatus();
	messages.push_back(this->getResultMessageForPlayer(this->firstTeam->getFirstPlayer(), matchStatus));
	messages.push_back(this->getResultMessageForPlayer(this->firstTeam->getSecondPlayer(), matchStatus));
	messages.push_back(this->getResultMessageForPlayer(this->secondTeam->getFirstPlayer(), matchStatus));
	messages.push_back(this->getResultMessageForPlayer(this->secondTeam->getSecondPlayer(), matchStatus));

	if(this->wasCancelled){
		std::string team1 = std::to_string(this->firstTeam->getFirstPlayer()) + " - " + std::to_string(this->firstTeam->getSecondPlayer()); 
		std::string team2 = std::to_string(this->secondTeam->getFirstPlayer()) + " - " + std::to_string(this->secondTeam->getSecondPlayer()); 
		std::string message = "Match: partido cancelado entre el equipo: " + team1 + " y el equipo: " + team2;
		log(message,INFORMATION);
	}


	return messages;
};

int Match::getMatchStatus() {
	int status = 0;
	if (this->wasCancelled) {
		status = gameCanceled;
	} else {
		status = gameCompleted;
	}
	return status;
};

messagePlayer Match::getResultMessageForPlayer(int player, int matchStatus) {
	struct messagePlayer message;
	message.idPlayer = player;
	message.status = matchStatus;
	return message;
};

void Match::cancelMatch() {
	this->wasCancelled = true;
};

bool Match::wasMatchCancelled() {
	return this->wasCancelled;
};

bool Match::isFinished() {
	return this->matchFinished;
}
