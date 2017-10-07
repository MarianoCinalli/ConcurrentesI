#include <string>

#ifndef MESSAGES_H_
#define MESSAGES_H_

	/*
	*	struct sent to fifoPlayerManager, used by PlayerManager.
	*	CommandManager and Court send this type of struct
	*	if CommandManager send this message PlayerManager ignore idPlayer
	*	types of status: remove, add, finalized, disrupted
	*
	*
	* 	this struct also is sent to FifoTeamManager,
	*	PlayerManager send this struct to notify status 
	*	of last Game the player, to TeamManager.
	*
	*/
	struct messagePlayer {
		int idPlayer;
		std::string status;
	};


	/*
	*	TeamManager send this type of struct.
	*	struct sent to fifoMatchManager, fifo used for MatchManager 
	*	
	*/
	struct messageTeam {
		int idPlayer1;
		int idPlayer2;
	};


	/*
	*	MatchManager send this type of struct.
	*	struct sent to fifoMatches, used for all Courts
	*	
	*/
	struct messageMatch {
		int idPlayer1_Team1;
		int idPlayer2_team1;
		int idPlayer1_Team2;
		int idPlayer2_team2;
	};



	/*
	*	struct sent to fifoResultManager, used for resultManager
	*	each court send this type of struct when its end successtly.
	*	
	*/
	struct messageResult {
		int idPlayer1_Team1;
		int idPlayer2_team1;
		int idPlayer1_Team2;
		int idPlayer2_team2;

		int setsWonTeam1;
		int setsWonTeam2;
	};

#endif // MESSAGES_H_