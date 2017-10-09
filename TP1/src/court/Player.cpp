#include "court/Player.h"

Player::Player() {

};

Player::~Player() {

};

void Player::addPoints(int points) {
	this->points += points;
}

int Player::getPoints() {
	return this->points;
}
