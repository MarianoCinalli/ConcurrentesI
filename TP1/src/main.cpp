#include <iostream>
#include <string>
#include <unistd.h>
#include "tools/logger.h"
#include "tools/utilFunctions.h"
#include "tools/ProcessSpawner.h"
#include "courtManager/CourtManager.h"

// Del branch courtManager ------------------------------
void openCourtManager(int parameters[]) {
	// 0 rows - 1 columns
	CourtManager* courtManager = new CourtManager(parameters[0], parameters[1]);
	courtManager->administrateCourts();
	delete(courtManager);
}
// ------------------------------------------------------

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
    // Del branch courtManager ------------------------------
    int parsedRows = 3; // Reemplazar por el valor parseado
    int parsedColumns = 3; // Reemplazar por el valor parseado
    int parameters[2] = {parsedRows, parsedColumns};
	ProcessSpawner* spawner = new ProcessSpawner();
	pid_t newProcessPid = spawner->spawnProcess(&openCourtManager, parameters);
    // End Main body
	logSessionFinished();
	return 0;
}
