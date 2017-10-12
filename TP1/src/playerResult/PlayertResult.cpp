#include"../headers/playerResult/PlayerResult.h"
#include<string>

PlayerResult::PlayerResult(int id) {
    log(PLAYER_RESULT_NAME + " Se crea nuevo Player Result ", INFORMATION);
    this->id = id;
}

void PlayerResult::showScore() {
    log(PLAYER_RESULT_NAME + " Se muestra el score del jugador " + std::to_string(this->id), INFORMATION);
    std::cout<<"El jugador " + std::to_string(this->id) + " tiene un puntaje de " + std::to_string(this->score) <<std::endl;
}

void PlayerResult::addScore(int score) {
    this->score += score;
}

PlayerResult::~PlayerResult() {}