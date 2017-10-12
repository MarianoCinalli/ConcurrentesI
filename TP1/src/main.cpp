#include <iostream>
#include <string>
#include "tools/logger.h"
#include <unistd.h>

// TODO: Delete when finished.
#include "tools/utilFunctions.h"
#include <sys/wait.h>
#include "tools/ProcessSpawner.h"
#include "court/Court.h"

void openCourtManager(int parameters[]) {
	// 0 rows - 1 columns
	CourtManager* courtManager = new CourtManager(parameters[0], parameters[1]);
	courtManager->administrateCourts();
	delete(courtManagerl);
}

void openCourt() {
	Court* court = new Court();
	court->runUntilThereAreNoMatchesLeft();
	delete(court);
}
// -------------------------------

// Constants ------------------------------------------------------
int LOG_MIN_LEVEL = 1;
std::ofstream LOG_FILE_POINTER;
// Constants ------------------------------------------------------

int main(int argc, char* argv[]) {
    // Initialization
    srand(time(NULL)); // Init seed for random
    LOG_FILE_POINTER.open("beachVoley.log", std::ofstream::app);
    // End Initialization
    logSessionStarted();
    // Main body

    // TODO: Delete when finished. -------------------------
	ProcessSpawner* spawner = new ProcessSpawner();
	pid_t newProcessPid = spawner->spawnProcess(&openCourt);
	int status;
	waitpid(newProcessPid, &status, WUNTRACED);
	// -----------------------------------------------------

    // End Main body
	logSessionFinished();
	return 0;
}
