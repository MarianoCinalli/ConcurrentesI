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
enum CommandType {addType=0, removeType=1, killType=2,gameCanceled=3,gameCompleted=4};
enum CourtCommands {PLAY = 0, CLOSE = 1};
enum TideType {raiseType=0, lowType=1};
enum LevelsLog{ERROR=1, WARNING, INFORMATION};

//fifos usados por Command
const std::string FIFO_WRITE_COMMAND_TO_PLAYERMANAGER = "/tmp/file_fifo_CommandManager_PlayerManager";
const std::string FIFO_WRITE_COMMAND_TO_COURTMANAGER = "/tmp/file_fifo_CommandManager_CourtManager";

//fifos usados por PlayerManager
const std::string FIFO_READ_COMMAND_OF_COMMANDMANAGER = FIFO_WRITE_COMMAND_TO_PLAYERMANAGER;
const std::string FIFO_WRITE_PLAYER_TO_TEAMMANAGER = "/tmp/file_fifo_PlayerManager_TeamManager";

//fifos usados por TeamManager
const std::string FIFO_READ_PLAYER_OF_PLAYERMANAGER = FIFO_WRITE_PLAYER_TO_TEAMMANAGER;
const std::string FIFO_WRITE_TEAM_TO_MATCHMANAGER = "/tmp/file_fifo_TeamManager_MatchManager";


//fifos usados por MatchManager
const std::string FIFO_READ_TEAM_OF_TEAMMANAGER = FIFO_WRITE_TEAM_TO_MATCHMANAGER;
const std::string FIFO_WRITE_MATCH_TO_MATCHES = "/tmp/fifoMatches";

//fifos usados por Court
const std::string FIFO_READ_MATCH_OF_MATCHMANAGER = FIFO_WRITE_MATCH_TO_MATCHES;
const std::string FIFO_WRITE_RESULT_TO_RESULTMANAGER = "/tmp/fifoResultManager";
const std::string FIFO_WRITE_STATUS_TO_PLAYERMANAGER = "/tmp/fifoPlayerManager";


#endif // CONSTANTS_H_

