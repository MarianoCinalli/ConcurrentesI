#include"../headers/playerResult/PlayerResult.h"
#include<string>

PlayerResult::PlayerResult(int id) {
    log(PLAYER_RESULT_NAME + " : Se crea nuevo Player Result ", INFORMATION);
    this->id = id;
    this->score = 0;
}

void PlayerResult::showScore() {
    log(PLAYER_RESULT_NAME + " : El jugador " + std::to_string(this->id) + " tiene un puntaje de " + std::to_string(this->score), INFORMATION);
    std::cout<<"El jugador " + std::to_string(this->id) + " tiene un puntaje de " + std::to_string(this->score) <<std::endl;
}

void PlayerResult::addScore(int score) {
    this->score += score;
}

int PlayerResult::getScore() {
    return this->score;
}

int PlayerResult::getId() {
    return this->id;
}

PlayerResult::~PlayerResult() {
    log(PLAYER_RESULT_NAME + " : Se liberan recursos ", INFORMATION);
}