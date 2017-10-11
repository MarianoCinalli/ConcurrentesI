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
