#include "tools/ProcessSpawner.h"
#include <vector>
#include "tools/Constants.h"

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
		wait(NULL);
	}
	return 0;
}


// Ejecuta N cantidad de de funciones
// Al terminar todas las ejecucion de cada funcion el nuevo proceso finaliza. 
int ProcessSpawner::spawnProcesses(std::vector<functiontype>*funtions) {

	pid_t pid;
	std::vector<functiontype>::iterator it;
	int totalFunctions = funtions->size();
	log("creando funciones hijo ", totalFunctions, 3);
	bool isChild = false;

	for(it = funtions->begin();it != funtions->end();it++){
		functiontype function = (*it);
		pid = fork();
		isChild = pid == 0;
		if (isChild) {
			function(); 
			exit ( 0 );
		} 
	}

	for(int i = 0; i < totalFunctions; i++) {
		log("Proceso creado, su pid es ", pid, 3);
		this->pids.push_back(pid);
		wait(NULL);
	}

	return 0;
}

