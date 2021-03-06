#include <iostream>
#include <string>
#include <unistd.h>

#include "tools/ProcessSpawner.h"
#include "TeamManager.h"
#include "MatchManager.h"
#include "playerManager/PlayerManager.h"
#include "playerResult/ResultManager.h"

// TODO: Delete when finished.
#include "tools/logger.h"
#include "tools/utilFunctions.h"
#include "tools/ProcessSpawner.h"
#include "courtManager/CourtManager.h"
#include "semaphores/Semaforo.h"
#include "fifos/FifoInitializer.h"

// Del branch courtManager ------------------------------
void openCourtManager(int parameters[]) {
	// 0 rows - 1 columns
	CourtManager* courtManager = new CourtManager(parameters[0], parameters[1]);
	courtManager->administrateCourts();
	delete(courtManager);
}

/* LO QUE USA ESTO ESTA COMENTADO DEBERIA VOLAR.
void openCourt() {
    log("INICIO DEL COURT",INFORMATION);
	Court* court = new Court();
    court->runUntilThereAreNoMatchesLeft();
    delete(court);
    log("FIN DEL COURT",INFORMATION);
}
// ------------------------------------------------------
*/

// Constants ------------------------------------------------------
const char INITIAL_PARAMETERS[] = "initialParameter.json";
int LOG_MIN_LEVEL = 1;
std::ofstream LOG_FILE_POINTER;
const std::string SEMAPHORE_NAME = "src/main.cpp";
Semaforo semaphoreFifoMatches(SEMAPHORE_NAME, 0);
// Constants ------------------------------------------------------

void executePlayerManager(){
    log("INICIO DEL PLAYER_MANAGER",INFORMATION);
    struct initialParameter *initialParameters = loadInitialParameters(INITIAL_PARAMETERS);
    PlayerManager *playerManager = new PlayerManager(initialParameters->maxPlayer , initialParameters->maxGamesByPlayer);
    playerManager->execute();
    delete initialParameters;
    delete playerManager;
    log("FIN DEL PLAYER_MANAGER",INFORMATION);
}

void executeTeamManager(){
    log("INICIO DEL TEAM_MANAGER",INFORMATION);
    TeamManager *teamManager = new TeamManager();
    teamManager->execute();
    delete teamManager;
    log("FIN DEL TEAM_MANAGER",INFORMATION);
}

void executeMatchManager(){
    log("INICIO DEL MATCH_MANAGER",INFORMATION);
    MatchManager *matchManager = new MatchManager();
    matchManager->execute();
    delete matchManager;
    log("FIN DEL MATCH_MANAGER",INFORMATION);
}

void executeResultManager(){
    log("INICIO DEL RESULT_MANAGER",INFORMATION);
    ResultManager *resultManager = new ResultManager();
    resultManager->execute();
    delete resultManager;
    log("FIN DEL RESULT_MANAGER",INFORMATION);
}




typedef void (*functiontype)();

int main(int argc, char* argv[]) {
    // Initialization
    FifoInitializer* fifoInitializer = new FifoInitializer();
    fifoInitializer->createFifos();
    srand(time(NULL)); // Init seed for random
    LOG_FILE_POINTER.open("beachVoley.log", std::ofstream::app);
    // End Initialization

    std::cout<<"COMIENZA EJECUCION"<<std::endl;

//    static const string NOMBRE1 = "PlayerManagerCourt";
 //   Semaforo semaforoPlayerCourt ( NOMBRE1,0 );

    logSessionStarted();
    struct initialParameter* initialParameters = loadInitialParameters(INITIAL_PARAMETERS);
    log("INICIO DEL PROCESO PRINCIPAL",INFORMATION);
    ProcessSpawner *processSpawner = new ProcessSpawner();
    std::vector<functiontype> *functions = new std::vector<functiontype>();
    
    functions->push_back(executePlayerManager);
    functions->push_back(executeTeamManager);
    functions->push_back(executeMatchManager);
    //functions->push_back(openCourt);
    functions->push_back(executeResultManager);
 //   ProcessSpawner *processSpawner = new ProcessSpawner();
//    ProcessSpawner *processSpawner = new ProcessSpawner();
    processSpawner->spawnProcesses(functions);

 
    // Del branch courtManager ------------------------------
    int parsedRows = initialParameters->rows;
    int parsedColumns = initialParameters->columns;
    int parameters[2] = {parsedRows, parsedColumns};
    ProcessSpawner* spawner = new ProcessSpawner();
    pid_t newProcessPid = spawner->spawnProcess(&openCourtManager, parameters);
    // End Main body

    // Esto se podria hacer constante y que se borre NOMBRE_FIFO_MATCHES + "_lock".
    // Porque desde el destructor no se puede borrar (la borra al primero que lo ejecuta).
    std::string lockFile = "/tmp/fifoMatches_lock";
    remove(lockFile.c_str());

    // End Main body

    
    processSpawner->waitChilds();
    delete initialParameters;
    delete functions;
    log("FIN DEL PROCESO PRINCIPAL",INFORMATION);


    // Main body
    // TODO: Delete when finished. -------------------------
  /*  int parameters[2] = { 1, 1 };
	ProcessSpawner* spawner = new ProcessSpawner();
	pid_t newProcessPid = spawner->spawnProcess(&openCourtManager, parameters);
	int status;
	waitpid(newProcessPid, &status, WUNTRACED);
	// -----------------------------------------------------
*/


/*
// Del branch courtManager ------------------------------
    int parsedRows = 3; // Reemplazar por el valor parseado
    int parsedColumns = 3; // Reemplazar por el valor parseado
    int parameters[2] = {parsedRows, parsedColumns};
	ProcessSpawner* spawner = new ProcessSpawner();
	pid_t newProcessPid = spawner->spawnProcess(&openCourtManager, parameters);
    // End Main body
*/


/*
    // EJEMPLO DE PARSER
    
    struct initialParameter* initialParameters = loadInitialParameters(INITIAL_PARAMETERS);
    log("rows : " + std::to_string(initialParameters->rows),INFORMATION);
    log("columns : " + std::to_string(initialParameters->columns),INFORMATION);
    log("maxPlayer : " + std::to_string(initialParameters->maxPlayer),INFORMATION);
    log("maxGamesByPlayer : " + std::to_string(initialParameters->maxGamesByPlayer),INFORMATION);
    delete initialParameters;

*/
    fifoInitializer->deleteFifos();
    delete fifoInitializer;
	logSessionFinished();
	return 0;
}
