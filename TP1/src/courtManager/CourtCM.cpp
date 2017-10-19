#include "courtManager/CourtCM.h"

// Constructor
CourtCM::CourtCM(){
	this->openCourt();
};

void CourtCM::openCourt() {
	log("CourtCM: Abriendo cancha", 3);
	ProcessSpawner* spawner = new ProcessSpawner();
	this->courtPid = spawner->spawnProcess(&(CourtCM::createCourt));
	this->isOpen = true;
	log("CourtCM: Cancha abierta. Nro de cancha ", this->courtPid, 3);
};

void CourtCM::closeCourt() {
	log("CourtCM: Clausurando cancha", 3);
	kill(this->courtPid, SIGINT);
	this->isOpen = false; 
	log("CourtCM: Cancha clausurada. Nro de cancha", this->courtPid, 3);
};

bool CourtCM::isCourtOpen() {
	return this->isOpen;
};

// Destructor
CourtCM::~CourtCM(){

};
