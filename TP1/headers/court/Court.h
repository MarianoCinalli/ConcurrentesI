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
#include "semaphores/Semaforo.h"
#include "court/Message.h"
#include "court/Match.h"
#include "court/Team.h"
#include "court/signal/SignalHandler.h"

class Court {

	// Atributes ---------------------------------------------
	private:
		FifoLecturaBloqueante* fifoMatches;
		FifoEscritura* fifoResults;
		FifoEscritura* fifoPlayerManager;
		bool matchShouldBeCancelled;
		bool moreMatchesToPlay;
		struct messageMatch readMessage;
		Match* match;

	// Methods -----------------------------------------------
	public:
		Court();
		~Court();
		void runUntilThereAreNoMatchesLeft();
		Match* getMatchFromMessage(Message* message);
		void sendMessages(Match* match);
		messageMatch getReadMessage();
		Match* getMatch();

	private:
		Message* getMessage();
		bool processMessage(Message* message);
		void playGame(Message* message);
};

#endif // COURT_H
