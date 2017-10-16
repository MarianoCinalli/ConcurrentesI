#ifndef COURT_H
#define COURT_H

#include <errno.h>
#include <string.h>
#include <vector>
#include <signal.h>
#include "tools/logger.h"
#include "tools/Messages.h"
#include "tools/Constants.h"
#include "fifos/FifoEscritura.h"
#include "fifos/FifoLecturaBloqueante.h"
#include "court/Message.h"
#include "court/Match.h"
#include "court/Team.h"

class Court {

	// Atributes ---------------------------------------------
	private:
		FifoLecturaBloqueante* fifoMatches;
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
		static void handler_cleanAndExit(int signum);
		void registerHandler();
		Match* getMatchFromMessage(Message* message);
};

#endif // COURT_H
