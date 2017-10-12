#include "../headers/playerManager/PlayerPM.h"

PlayerPM::PlayerPM(int id){
	this->id = id;
	this->status = freeS;
	this->gamesPlayed = 0;
}

PlayerPM::PlayerPM(int id, PCStatus status ){
	this->id = id;
	this->status = status;
}

PlayerPM::~PlayerPM(){
}

std::string PlayerPM::logMemberVariables() {
	return "Jugador = " + std::to_string(this->id)
	+ "- partidos jugados = " + std::to_string(this->gamesPlayed)
	+ "- estado = " + std::to_string(this->status);
}