#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <iostream>

static const int F_DEFAULT = 4;
static const int C_DEFAULT = 4;
static const int MAX_PLAYERS_DEFAULT = 10;
static const int MAX_GAMES_PER_PLAYER_DEFAULT = 3;
static const int MIN_SETS_DEFAULT = 3;
static const int MAX_SETS_DEFAULT = 5;

enum PCStatus {freeS=0, busyS, disableS,completedS};
enum CommandType {addType=0, removeType=1, killType,gameCanceled,gameCompleted};
enum TideType {raiseType=0, lowType=1};
enum LevelsLog{INFORMATION=1,WARNING,ERROR};

//fifos usados por Command
const std::string FIFO_WRITE_COMMAND_TO_PLAYER = "/tmp/file_fifo_CommandManager_PlayerManager";
const std::string FIFO_WRITE_COMMAND_TO_COURT = "/tmp/file_fifo_CommandManager_CourtManager";

//fifos usados por PlayerManager
const std::string FIFO_READ_PLAYER_OF_COMMAND = FIFO_WRITE_COMMAND_TO_PLAYER;
const std::string FIFO_WRITE_PLAYER_TO_TEAM = "/tmp/file_fifo_PlayerManager_TeamManager";

//fifos usados por TeamManager
const std::string FIFO_READ_TEAM_OF_PLAYER = FIFO_WRITE_PLAYER_TO_TEAM;
const std::string FIFO_WRITE_TEAM_TO_MATCH = "/tmp/file_fifo_TeamManager_MatchManager";


/*
const std::string FILE_FIFO_READ = "/tmp/file_fifo_CommandManager_PlayerManager";
const std::string FILE_FIFO_WRITE = "/tmp/file_fifo_PlayerManager_TeamManager";

const std::string FILE_FIFO_TEAM_MANAGER_MATCH_MANAGER = "/tmp/file_fifo_PlayerManager_TeamManager";
const std::string FILE_FIFO_MATCH_MANAGER_COURT_MANAGER = "/tmp/file_fifo_MatchManager_CourtManager";
*/
#endif // CONSTANTS_H_