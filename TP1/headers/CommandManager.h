
/*
* commmandManager: Read from STDIN and interprete the commands of user
* 
*
*/
#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include <string>
#include "fifos/FifoEscritura.h"
#include "tools/Messages.h"
#include "tools/Constants.h"
#include "interfaces/Executable.h"
#include "tools/logger.h"

class CommandManager : public Executable {

private:

	/*
	* Pre: The Village has to be created and initialized
	*		The PlayerManager has that be create and initialized
	*
	* Post: Send a message to fifo fifoPlayerManager to add one player to game:
	* 		if Village has capacity a player is added to the same.
	*		else the player is in state of waiting to into to Village
	*/
	void addPlayer();

	/*
	* Pre: The Village has to be created and initialized
	*		The PlayerManager has that be create and initialized
	*
	* Post: Send a message to fifo fifoPlayerManager to remove one player to game:
	*		the player has to be in status free to be removed
	* 		if Village hasn't capacity this player isn't removed
	*
	*/
	void removePlayer();

	/*
	* Pre: The Village, PlayerManager,CourtManager and all Courts  
	*		have to be created and initialized
	*
	* Post: Send a message to CourtManager report that the tide is rising
	*		finalizing to the courts' matches affected and disables them indefinitely
	*/
	void raiseTide();

	/*
	* Pre: The Village, PlayerManager,CourtManager and all Courts  
	*		have to be created and initialized

	*
	* Post: Send a message to CourtManager report that the tide is lowing
	* 		enables a row the courts of the Village.
	*/	
	void lowTide();

	void receiveCommand(char command);

	bool finalizedProcess;
	FifoEscritura *fifoManagerPlayer;
	FifoEscritura *fifoTide;

public:

	CommandManager();
	~CommandManager();
	void execute();

};
#endif // COMMANDMANAGER_H
