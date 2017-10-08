
#include "Player.h"

Player::Player(int id){
	this->id = id;
	this->status = freeS;
	this->gamesPlayed = 0;
}

Player::Player(int id, PCStatus status ){
	this->id = id;
	this->status = status;
}

Player::~Player(){
}
