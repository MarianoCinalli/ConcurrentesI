#ifndef COURT_H
#define COURT_H

#include <errno.h>
#include <string.h>
#include <vector>
#include "tools/logger.h"
#include "tools/Messages.h"
#include "tools/Constants.h"
#include "fifos/FifoLectura.h"
#include "fifos/FifoEscritura.h"
#include "court/Message.h"
#include "court/Match.h"
#include "court/Team.h"

class Court {

	// Atributes ---------------------------------------------
	private:
		FifoLectura* fifoMatches;
		FifoEscritura* fifoResults;
		FifoEscritura* fifoPlayerManager;
		bool matchShouldBeCancelled;

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
};

#endif // COURT_H
