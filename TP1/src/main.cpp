#include <iostream>
#include <string>
#include <unistd.h>
#include "tools/logger.h"
#include "tools/utilFunctions.h"
#include "tools/ProcessSpawner.h"
#include "courtManager/CourtManager.h"
#include "semaphores/Semaforo.h"

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
const std::string SEMAPHORE_NAME = "src/main.cpp";
Semaforo semaphoreFifoMatches(SEMAPHORE_NAME, 0);
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

    // Esto se podria hacer constante y que se borre NOMBRE_FIFO_MATCHES + "_lock".
    // Porque desde el destructor no se puede borrar (la borra al primero que lo ejecuta).
    std::string lockFile = "/tmp/fifoMatches_lock";
    remove(lockFile.c_str());
	return 0;
}
