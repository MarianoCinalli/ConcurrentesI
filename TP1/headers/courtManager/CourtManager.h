#ifndef COURTMANAGER_H
#define COURTMANAGER_H

#include <vector>
#include <string.h>
#include "tools/Constants.h"
#include "tools/logger.h"
#include "tools/Messages.h"
#include "fifos/FifoLectura.h"
#include "fifos/FifoEscritura.h"
#include "semaforos/Semaforo.h"
#include "courtManager/CourtCM.h"
#include "courtManager/MessageCM.h"

class CourtManager {

	// Atributes ------------------------------------------------------
	private:
		std::vector<std::vector<CourtCM*>> beach;
		int tideLevel;
		int rows;
		int columns;
		FifoLectura* fifoCourtManager;

	// Public methods -------------------------------------------------
	public:
		CourtManager(int rows, int columns);
		~CourtManager();
		void administrateCourts();

	// Private methods ------------------------------------------------
	private:
		void initializeCourts();
		MessageCM* getMessage();
		bool processMessage(MessageCM* message);
		void raiseTideAndInformCourts(int amountToRaise);
		void lowerTideAndInformCourts(int amountToLower);
		int getUpdatedTideLevel(int amountToAdd);
		void shutDownCourts();
};

#endif // COURTMANAGER_H
