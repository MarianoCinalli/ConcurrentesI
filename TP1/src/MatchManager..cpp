#include "MatchManager.h"

MatchManager::MatchManager() {
    this->channelToReadTeams = new FifoLectura(FIFO_READ_TEAM_OF_TEAMMANAGER);
    this->channelToWriteMatches = new FifoEscritura(FIFO_WRITE_MATCH_TO_MATCHES);
    this->team1 = NULL;
    this->team2 = NULL;
}

void MatchManager::execute() {
    this->channelToReadTeams->abrir();
    this->channelToWriteMatches->abrir();

    while (!this->finalize) {
        struct messageTeam* team = this->readTeam();
        this->parseMessage(team);
    }
    log("MatchManager ha finalizado correctamente",INFORMATION);
}




void MatchManager::parseMessage(struct messageTeam* team) {
    
    switch(team->operation){

        case PLAY :
            this->notifyMatch(team);
            log("llega un team al matchManager",INFORMATION);
            break;
        case CLOSE :
            this->finalize = true;
            std::cout<<"-----antes de notificar el close------"<<std::endl;
            flushLog(); //agregadoooooo
            this->notifyCloseMatches();
            std::cout<<"-----llego un mensaje close------"<<std::endl;
            log("llega un close al matchManager",INFORMATION);
            break;
        
    }
}


void MatchManager::notifyMatch(struct messageTeam* team){
    if(this->team1 == NULL){
        this->team1 = team;
    }else{
        this->team2 = team;
        struct messageMatch *match = this->makeMatch(team1, team2);
        delete this->team1;
        delete this->team2;
        this->writeMatch(match);
        delete match;
        this->team1 = NULL;
        this->team2 = NULL;
    }
}


void MatchManager::notifyCloseMatches(){
    struct messageMatch * match = new messageMatch;
    match->operation = CLOSE;
    std::cout<<"-----errro solo escribe en log con los couts------"<<std::endl;
    //std::cout<<"-----antes del write-----"<<std::endl;
    this->writeMatch(match);
    //std::cout<<"-----despues del write------"<<std::endl;
}


messageMatch* MatchManager::makeMatch(struct messageTeam* team1, struct messageTeam* team2) {
    struct messageMatch *match = new messageMatch;
    match->idPlayer1_Team1 = team1->idPlayer1;
    match->idPlayer2_team1 = team1->idPlayer2;
    match->idPlayer1_Team2 = team2->idPlayer1;
    match->idPlayer2_team2 = team2->idPlayer2;
    log("se ha creado un match",INFORMATION);
    log("jugador ",team1->idPlayer1,INFORMATION);
    log("jugador ",team1->idPlayer2,INFORMATION);
    log("jugador ",team2->idPlayer1,INFORMATION);
    log("jugador ",team2->idPlayer2,INFORMATION);
    return match;
}

void MatchManager::writeMatch(struct messageMatch *match) {
    int result = this->channelToWriteMatches->escribir(match,sizeof(messageMatch));
    if(result == -1){
        log(MATCH_MANAGER_NAME + " : No se pudo realizar la escritura en el fifo ", __FILE__, __LINE__, ERROR);
    }else if (result != sizeof(messageMatch)){
        log(MATCH_MANAGER_NAME + " : Se ha escrito una cantidad erronea de bytes en el fifo ", __FILE__, __LINE__, ERROR);
    }
}

messageTeam* MatchManager::readTeam() {
    struct messageTeam *buff = new messageTeam;
	memset(buff,0,sizeof(messageTeam));

    int result = this->channelToReadTeams->leer(buff,sizeof(messageTeam));
    
	if(result == -1){
		log(MATCH_MANAGER_NAME + " : No se pudo realizar la lectura del fifo ",__FILE__, __LINE__, ERROR);
	}else if (result != sizeof(messageTeam)){
		log(MATCH_MANAGER_NAME + " : Se ha leido una cantidad erronea de bytes del fifo ",__FILE__, __LINE__, ERROR);
    }
    
    return buff;
}

MatchManager::~MatchManager() {
    this->channelToReadTeams->cerrar();
    this->channelToWriteMatches->cerrar();    
    delete this->channelToReadTeams;
    delete this->channelToWriteMatches;
    
}