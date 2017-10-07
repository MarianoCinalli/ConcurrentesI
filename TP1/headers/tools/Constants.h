#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <iostream>

static const int F_DEFAULT = 4;
static const int C_DEFAULT = 4;
static const int MAX_PLAYERS_DEFAULT = 10;
static const int MAX_GAMES_PER_PLAYER_DEFAULT = 3;
static const int MIN_SETS_DEFAULT = 3;
static const int MAX_SETS_DEFAULT = 5;

enum PCStatus {freeS=0, busyS, disableS};
enum CommandType {addType=0, removeType=1};
enum TideType {raiseType=0, lowType=1};


struct Player {
    int id;
    std::string name;
    PCStatus status;
    int score;
};

struct Team {
    int id;
    int idPlayer1;
    int idPlayer2;
    int setsWon;
    int setsLost;
};

struct Court {
    int id;
    PCStatus status;
};

#endif // CONSTANTS_H_