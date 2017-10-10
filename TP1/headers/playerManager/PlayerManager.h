#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

#include "../fifos/FifoLectura.h"
#include "../fifos/FifoEscritura.h"
#include <vector>
#include "Player.h"
#include <string.h>
#include "../tools/utilFunctions.h"
#include "../tools/Constants.h"
#include "../tools/Messages.h"
#include "../tools/logger.h"

class PlayerManager {

	private:

		bool finalizedProcess;
		FifoLectura *channelToRead;
		FifoEscritura *channelToWrite;
		int idPlayer;
		int maxPlayersVillage;
		int maxMatchesPerPlayer;
		std::vector<Player*> *playersToGame;
		std::vector<Player*> *playersToWait;
		int generateId();
		void removePlayersWithGamesCompleted();
		void removePlayerToGame();
		void addPlayerToGame();
		struct messagePlayer* readFifoPlayerManager();
		void writeFifoTeamManager();
		void parseMessage(struct messagePlayer* message);
		void writeMessagePlayer(struct messagePlayer* message);
		void updateMatchesPlayer(int idPlayer);

	public:
		PlayerManager(int maxPlayersVillage, int maxMatchesPerPlayers);
		~PlayerManager();
		void execute();
};

#endif // PLAYERMANAGER_H
