#include "court/Team.h"

Team::Team(int firstPlayer, int secondPlayer) {
	this->firstPlayer = firstPlayer;
	this->secondPlayer = secondPlayer;
	this->setsWon = 0;
};

Team::~Team() {

};

int Team::getFirstPlayer(){
	return this->firstPlayer;
};

int Team::getSecondPlayer(){
	return this->secondPlayer;
};

int Team::getSetsWon() {
	return this->setsWon;
}

void Team::setWonSets(int setsWon) {
	this->setsWon = setsWon;
};

std::string Team::logMemberVariables() {
	return "Primer jugador = " + std::to_string(this->firstPlayer)
	+ " - Segundo jugador = " + std::to_string(this->secondPlayer)
	+ " - Sets ganados = " + std::to_string(this->setsWon);
};

