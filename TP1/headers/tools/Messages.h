#include <iostream>
#include <string>

#ifndef MESSAGE_H
#define MESSAGE_H


class MESSAGE_H{

	/*
	*	struct sent to fifoPlayerManager, used by PlayerManager.
	*	
	*	CommandManager and Court sent this type of struct
	*	if CommandManager send this message PlayerManager ignore idPlayer
	*	
	*	types of status: remove, add, finalized, disrupted
	*/
	struct messagePlayer {
		int idPlayer;
		std::string status;
	};


	/*
	*	struct sent to fifoTeamManager, used for TeamManager 
	*
	*	PlayerManager send this type of struct
	*	
	*/
	struct messageTeam {
		int idPlayer1;
		int idPlayer2;
	};


	struct messageMatch {
		int idPlayer1_Team1;
		int idPlayer2_team1;
		int idPlayer1_Team2;
		int idPlayer2_team2;
	};


	struct messageResult {
		int idPlayer1_Team1;
		int idPlayer2_team1;
		int idPlayer1_Team2;
		int idPlayer2_team2;

		int setsWonTeam1;
		int setsWonTeam2;
	};


};