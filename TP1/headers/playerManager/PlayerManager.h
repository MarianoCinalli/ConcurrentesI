#include "../fifos/FifoLectura.h"
#include "../fifos/FifoEscritura.h"
#include <vector>
#include "Player.h"
#include <string.h>
#include "../tools/utilFunctions.h"
#include "../tools/Constants.h"
#include "../tools/Messages.h"
#include "../tools/logger.h"

#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

//fifo que se usa para comunicarse con el CommandManager y  Court
const std::string FILE_FIFO_READ = "/tmp/file_fifo_CommandManager_PlayerManager";
const std::string FILE_FIFO_WRITE = "/tmp/file_fifo_PlayerManager_TeamManager";

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
