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
	return pid;
}

// Ejecuta N cantidad de de funciones
// Al terminar todas las ejecucion de cada funcion el nuevo proceso finaliza. 
int ProcessSpawner::spawnProcesses(std::vector<functiontype>*funtions) {

	pid_t pid;
	std::vector<functiontype>::iterator it;
	int totalFunctions = funtions->size();
	log("creando funciones hijo ", totalFunctions, 3);
	flushLog();
	bool isChild = false;

	for(it = funtions->begin();it != funtions->end();it++){
		functiontype function = (*it);
		pid = fork();
		isChild = pid == 0;
		if (isChild) {
			function();
			exit ( 0 );
		}else{
			log("Proceso creado, su pid es ", pid, 3);
			flushLog();
			this->pids.push_back(pid);
		} 
	}
/*
	for(int i = 0; i < totalFunctions; i++) {
		//this->pids.push_back(pid);
		pid = wait(NULL);
		if(pid != -1){
			log("Proceso finalizado, su pid es ", pid, 3);
		}else{
			log("Error en la finalizacion del proceso ", 3);
		}
		flushLog();
	}
*/
	return 0;
}


int ProcessSpawner::spawnProcess(functionWithIntParametersType functionPointer, int parameters[]) {
	pid_t pid;
	flushLog(); // Flusheo al log, porque sino pueden aparecer logs repetidos.
	pid = fork();
	if ( pid == 0 ) {
		// Hijo
		functionPointer(parameters);
		exit ( 0 );
	} else {
		// Padre
		log("Proceso creado, su pid es ", pid, 3);
		this->pids.push_back(pid);
	}
	return pid;

}

void ProcessSpawner::waitChilds(){
	pid_t pid;
	for(int i = 0; i < this->pids.size(); i++) {
		//this->pids.push_back(pid);
		pid = wait(NULL);
		if(pid != -1){
			log("Proceso finalizado, su pid es ", pid, 3);
		}else{
			log("Error en la finalizacion del proceso ", 3);
		}
		flushLog();
	}

	std::cout<<"-------------finalizaron todos los procesos"<<std::endl;

}