#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <iostream>

const int F_DEFAULT = 4;
const int C_DEFAULT = 4;
const int MAX_PLAYERS_DEFAULT = 10;
const int MAX_GAMES_PER_PLAYER_DEFAULT = 3;
const int MIN_SETS_DEFAULT = 3;
const int MAX_SETS_DEFAULT = 5;

enum PCStatus {freeS=0, busyS, disableS,completedS};
enum CommandType {addType=0, removeType=1, killType=2,gameCanceled=3,gameCompleted=4};
enum CourtCommands {PLAY = 0, CLOSE = 1};
enum TideType {raiseType=0, lowType=1, closeCourts=3};
enum LevelsLog{INFORMATION=1,WARNING,ERROR};

#endif // CONSTANTS_H_
