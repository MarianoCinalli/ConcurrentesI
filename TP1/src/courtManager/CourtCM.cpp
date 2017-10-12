#include "courtManager/CourtCM.h"

// Constructor
CourtCM::CourtCM(){
	this->openCourt();
};

void CourtCM::openCourt() {
	ProcessSpawner* spawner = new ProcessSpawner();
	this->courtPid = spawner->spawnProcess(&createCourt);
	this->isOpen = true;
};

void CourtCM::closeCourt() {
	kill(this->courtPid, SIGINT);
	this->isOpen = false; 
};

void CourtCM::createCourt() {
	Court* court = new Court();
	court->runUntilThereAreNoMatchesLeft();
	delete(court);
};

bool CourtCM::isOpen() {
	return this->isOpen;
};

// Destructor
CourtCM::~CourtCM(){

};
