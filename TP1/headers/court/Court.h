#include <errno.h>
#include <string.h>
#include <vector>
#include "tools/logger.h"
#include "tools/Messages.h"
#include "fifos/FifoLectura.h"
#include "fifos/FifoEscritura.h"
#include "court/Message.h"
#include "court/Match.h"
#include "court/Team.h"

#ifndef COURT_H
#define COURT_H

class Court {

	// Atributes ---------------------------------------------
	private:
		FifoLectura* fifoMatches;
		FifoEscritura* fifoResults;
		FifoEscritura* fifoPlayerManager;

	// Methods -----------------------------------------------
	public:
		Court();
		~Court();
		void runUntilThereAreNoMatchesLeft();

	private:
		Message* getMessage();
		bool processMessage(Message* message);
		void playGame(Message* message);
		void sendMessages(Match* match);
		void handler_cleanAndExit();
		void registerHandler();
		Match* getMatchFromMessage(Message* message);
};

#endif // COURT_H
