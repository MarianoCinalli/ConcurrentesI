#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

#include "../fifos/FifoLectura.h"
#include "../fifos/FifoEscritura.h"
#include <vector>
#include "PlayerPM.h"
#include <string.h>
#include "../tools/utilFunctions.h"
#include "../tools/Constants.h"
#include "../tools/Messages.h"
#include "../tools/logger.h"
#include <typeinfo>

class PlayerManager {

	const std::string PLAYER_MANAGER_NAME = typeid(PlayerManager).name();
	const unsigned minPlayersToBeginGame = 10;

	private:

		bool finalizedProcess;
		bool beginGame;
		unsigned removePlayer;
		FifoLectura *channelToRead;
		FifoEscritura *channelToWrite;
		FifoEscritura *channelToWriteResult;
		int idPlayer;
		unsigned maxPlayersVillage;
		unsigned maxMatchesPerPlayer;
		std::vector<PlayerPM*> *playersToGame;
		std::vector<PlayerPM*> *playersToWait;
		int generateId();
		void removePlayersWithGamesCompleted();
		void addPlayerToGame();
		struct messagePlayer* readFifoPlayerManager();
		void writeFifoTeamManager();
		void parseMessage(struct messagePlayer* message);
		void writeMessagePlayer(struct messagePlayer* message);
		void updateMatchesPlayer(int idPlayer);
		//void evaluteGamesCompletedPlayer(std::vector<PlayerPM*>::iterator it);
		bool evaluteGamesCompletedPlayer(PlayerPM *player);
		void removePlayerToGame();
		void notifyGameCanceled(struct messagePlayer* message);
		void loggearPlayers();
		void evaluateEndGame();
		void writeEndGameToResultManager();
		void executeCommandManager();


	public:
		PlayerManager(unsigned int maxPlayersVillage, unsigned int maxMatchesPerPlayers);
		~PlayerManager();
		void execute();
};

#endif // PLAYERMANAGER_H
