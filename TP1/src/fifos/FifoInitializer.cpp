#include "fifos/FifoInitializer.h"

void FifoInitializer::createFifos() {
	this->fifos = new std::vector<FifoLectura*>();
	this->fifos->push_back(new FifoLectura(FIFO_WRITE_COMMAND_TO_PLAYERMANAGER));
	this->fifos->push_back(new FifoLectura(FIFO_WRITE_COMMAND_TO_COURTMANAGER));
	this->fifos->push_back(new FifoLectura(FIFO_WRITE_PLAYER_TO_TEAMMANAGER));
	this->fifos->push_back(new FifoLectura(FIFO_WRITE_TEAM_TO_MATCHMANAGER));
	this->fifos->push_back(new FifoLectura(FIFO_WRITE_MATCH_TO_MATCHES));
	this->fifos->push_back(new FifoLectura(FIFO_WRITE_RESULT_TO_RESULTMANAGER));
}

void FifoInitializer::deleteFifos() {
    for (std::vector<FifoLectura*>::iterator it = fifos->begin(); it != fifos->end(); it++) {
        FifoLectura *fifo = (*it);
        fifo->eliminar();
        delete fifo;
    }
    fifos->clear();
    delete fifos;
}
