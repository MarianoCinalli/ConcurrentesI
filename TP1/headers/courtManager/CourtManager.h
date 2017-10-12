#include <vector>
#include <string.h>
#include "tools/Constants.h"
#include "tools/logger.h"
#include "tools/Messages.h"
#include "fifos/FifoLectura.h"
#include "fifos/FifoEscritura.h"
#include "courtManager/courtCM.h"
#include "courtManager/messageCM.h"

#ifndef COURTMANAGER_H
#define COURTMANAGER_H

class CourtManager {

	// Atributes ------------------------------------------------------
	private:
		std::vector<std::vector<CourtCM*>> beach;

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
		int getUpdatedTideLevel();
		void shutDownCourts();
};

#endif // COURTMANAGER_H
