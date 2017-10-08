#include "TeamManager.h"

TeamManager::TeamManager() {
    this->playsByPlayer = new std::map<int, std::vector<int>*>();
    this->fifoTeamManager = new FifoLectura(FILE_FIFO_PLAYER);
    this->fifoTeamManager->abrir();
}

void TeamManager::execute() {
    while (!this->finalize) {

    }
}

bool TeamManager::playersPlayBetween(int idPlayer1, int idPlayer2) {
    std::vector<int> *plays = this->playsByPlayer->at(idPlayer1);
    std::vector<int>::iterator it = std::find(plays->begin(), plays->end(), idPlayer2);
    if (it != plays->end())
      // std::cout << "Element found in myvector: " << *it << '\n';
      return true;
    else
      // std::cout << "Element not found in myvector\n";    
      return false;
}

messagePlayer* TeamManager::readPlayer() {
    //fifoManagerPlayer->leer();
}

TeamManager::~TeamManager() {
    std::map<int, std::vector<int>*>::iterator it;
    for(it = this->playsByPlayer->begin(); it != this->playsByPlayer->end(); it++) {
        std::vector<int> *plays = (*it).second;
        delete plays; 
    }
    delete this->playsByPlayer;
    this->fifoTeamManager->cerrar();
    delete this->fifoTeamManager;
}