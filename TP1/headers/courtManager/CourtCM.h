#ifndef COURTCM_H
#define COURTCM_H

#include "court/Court.h"
#include <sys/types.h>
#include <signal.h>
#include "tools/ProcessSpawner.h"
#include "tools/logger.h"

class CourtCM {

	// Atributes ------------------------------------------------------
	private:
		pid_t courtPid;
		bool isOpen;

	// Public methods -------------------------------------------------
	public:
		CourtCM();
		bool isCourtOpen();
		void openCourt();
		void closeCourt();
		~CourtCM();
		
		static void createCourt() {
			Court* court = new Court();
			court->runUntilThereAreNoMatchesLeft();
			delete(court);
		};

};

#endif // COURTCM_H
