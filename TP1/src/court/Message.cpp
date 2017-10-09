#include "court/Message.h"

Message::Message() {
	this->operation = 3;
};

Message::Message(struct messageMatch message) {
	this->firstTeamFirstPlayer = message.idPlayer1_Team1;
	this->firstTeamSecondPlayer = message.idPlayer2_team1;
	this->secondTeamFirstPlayer = message.idPlayer1_Team2;
	this->secondTeamSecondPlayer = message.idPlayer2_team2;
	this->operation = message.operation;
};

Message::~Message() {

};

int Message::getFirstTeamFirstPlayer() {
	return this->firstTeamFirstPlayer;
};

int Message::getFirstTeamSecondPlayer() {
	return this->firstTeamSecondPlayer;
};

int Message::getSecondTeamFirstPlayer() {
	return this->secondTeamFirstPlayer;
};

int Message::getSecondTeamSecondPlayer() {
	return this->secondTeamSecondPlayer;
};

int Message::getOperation() {
	return this->operation;
}

