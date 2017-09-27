#include "tools/ProcessSpawner.h"

extern std::ofstream LOG_FILE_POINTER;

ProcessSpawner::ProcessSpawner() {
};

ProcessSpawner::~ProcessSpawner() {
	
};

// Ejecuta la funcion en un nuevo proceso.
// Al terminar la ejecucion de la funcion el nuevo proceso finaliza. 
int ProcessSpawner::spawnProcess(functiontype functionPointer) {
	pid_t pid;
	flushLog(); // Flusheo al log, porque sino pueden aparecer logs repetidos.
	pid = fork();
	if ( pid == 0 ) {
		// Hijo
		functionPointer();
		exit ( 0 );
	} else {
		// Padre
		log("Proceso creado, su pid es ", pid, 3);
		this->pids.push_back(pid);
	}
	return 0;
}

