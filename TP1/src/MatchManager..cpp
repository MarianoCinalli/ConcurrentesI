#include "MatchManager.h"

MatchManager::MatchManager() {
    this->channelToReadTeams = new FifoLectura(FIFO_READ_TEAM_OF_TEAMMANAGER);
    this->channelToWriteMatches = new FifoEscritura(FIFO_WRITE_MATCH_TO_MATCHES);
}

void MatchManager::execute() {
    while (!this->finalize) {
        struct messageTeam* team1 = this->readTeam();
        struct messageTeam* team2 = this->readTeam();
        struct messageMatch *match = this->makeMatch(team1, team2);
        this->writeMatch(match);
    }
}

messageMatch* MatchManager::makeMatch(struct messageTeam* team1, struct messageTeam* team2) {
    struct messageMatch *match = new messageMatch;
    match->idPlayer1_Team1 = team1->idPlayer1;
    match->idPlayer2_team1 = team1->idPlayer2;
    match->idPlayer1_Team2 = team2->idPlayer1;
    match->idPlayer2_team2 = team2->idPlayer2;
    return match;
}

void MatchManager::writeMatch(struct messageMatch *match) {
    int result = this->channelToWriteMatches->escribir(match,sizeof(messageMatch));
    if(result == -1){
        log(MATCH_MANAGER_NAME + " : No se pudo realizar la escritura en el fifo ", __FILE__, __LINE__, ERROR);
    }else if (result != sizeof(messageMatch)){
        log(MATCH_MANAGER_NAME + " : Se ha escrito una cantidad erronea de bytes en el fifo ", __FILE__, __LINE__, ERROR);
    }
    delete match;
}

messageTeam* MatchManager::readTeam() {
    struct messageTeam *buff = new messageTeam;
	memset(buff,0,sizeof(messageTeam));

    int result = this->channelToReadTeams->leer(buff,sizeof(messageTeam));
    
	if(result == -1){
		log(MATCH_MANAGER_NAME + " : No se pudo realizar la lectura del fifo ","sss",100, 1);
	}else if (result != sizeof(messagePlayer)){
		log(MATCH_MANAGER_NAME + " : Se ha leido una cantidad erronea de bytes del fifo ","sss",100, 1);
    }
    
    return buff;
}

MatchManager::~MatchManager() {
    this->channelToReadTeams->cerrar();
    this->channelToWriteMatches->cerrar();    
    delete this->channelToReadTeams;
    delete this->channelToWriteMatches;
}