#include "court/Court.h"
#include <sys/types.h>
#include <signal.h>

#ifndef COURTCM_H
#define COURTCM_H

class CourtCM {

	// Atributes ------------------------------------------------------
	private:
		pid_t courtPid;
		bool isOpen;

	// Public methods -------------------------------------------------
	public:
		CourtCM();
		bool isOpen();
		void openCourt();
		void closeCourt();
		~CourtCM();
};

#endif // COURTCM_H
