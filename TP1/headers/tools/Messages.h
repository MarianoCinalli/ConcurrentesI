#include <string>

#ifndef MESSAGES_H_
#define MESSAGES_H_

	/*
	*	struct sent to fifoPlayerManager, used by PlayerManager.
	*	CommandManager and Court sent this type of struct
	*	if CommandManager send this message PlayerManager ignore idPlayer
	*	types of status: remove, add, finalized, disrupted
	*
	*
	* 	struct also sent to FifoTeamManager,
	*	PlayerManager sent this struct to notify status 
	*	of last Game the player, to TeamManager.
	*
	*/
	struct messagePlayer {
		int idPlayer;
		int status;
	};


	/*
	*	struct sent to fifoMatchManager, used for MatchManager 
	*	TeamManager send this type of struct.
	*	
	*/
	struct messageTeam {
		int idPlayer1;
		int idPlayer2;
	};


	/*
	*	struct sent to fifoMatches, used for all Courts
	*	MatchManager send this type of struct.
	*	
	*/
	struct messageMatch {
		int operation;
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

	struct messageCourtManager {
		int operation;
		int operationValue;
	}

#endif // MESSAGES_H_
