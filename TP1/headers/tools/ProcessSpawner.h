#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "tools/logger.h"

#ifndef PROCESSSPAWNER_H
#define PROCESSSPAWNER_H

typedef void (*functiontype)();
typedef void (*functionWithIntParametersType)(int parameters[]);

class ProcessSpawner {

private:
	std::vector<pid_t> pids;

public:
	ProcessSpawner();
	~ProcessSpawner();
	int spawnProcess(functiontype functionPointer);
	int spawnProcesses(std::vector<functiontype>*funtions);
	int spawnProcess(functionWithIntParametersType functionPointer, int parameters[]);
};

#endif // PROCESSSPAWNER_H
