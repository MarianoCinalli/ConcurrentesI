#include "MatchManager.h"

MatchManager::MatchManager() {
    this->channelToReadTeams = new FifoLectura(FIFO_READ_TEAM_OF_TEAMMANAGER);
    this->channelToWriteMatches = new FifoEscritura(FIFO_WRITE_MATCH_TO_MATCHES);
    this->team1 = NULL;
    this->team2 = NULL;
    this->channelToReadTeams->abrir();
    this->channelToWriteMatches->abrir();
    log("MatchManager: Se abrio FIFO de lectura " + FIFO_READ_TEAM_OF_TEAMMANAGER,INFORMATION);
    log("MatchManager: Se abrio FIFO de escritura " + FIFO_WRITE_MATCH_TO_MATCHES,INFORMATION);
}

void MatchManager::execute() {
//    this->channelToReadTeams->abrir();
//    this->channelToWriteMatches->abrir();

    while (!this->finalize) {
        struct messageTeam* team = this->readTeam();
        this->parseMessage(team);
    }
    log("MatchManager: MatchManager: el proceso ha finalizado correctamente",INFORMATION);
}




void MatchManager::parseMessage(struct messageTeam* team) {
    
    switch(team->operation){

        case PLAY :
            this->notifyMatch(team);
            log("MatchManager: llega un team ",INFORMATION);
            break;
        case CLOSE :
            this->finalize = true;
            flushLog(); //agregadoooooo porque el proceso no termina.......
            this->notifyCloseMatches();
            log("MatchManager: llega un close ",INFORMATION);
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
    this->writeMatch(match); //debbugear si se queda lokeado.....
    delete match;
}


messageMatch* MatchManager::makeMatch(struct messageTeam* team1, struct messageTeam* team2) {
    struct messageMatch *match = new messageMatch;
    match->idPlayer1_Team1 = team1->idPlayer1;
    match->idPlayer2_team1 = team1->idPlayer2;
    match->idPlayer1_Team2 = team2->idPlayer1;
    match->idPlayer2_team2 = team2->idPlayer2;
    match->operation = PLAY;
    return match;
}

void MatchManager::writeMatch(struct messageMatch *match) {
    int result = this->channelToWriteMatches->escribir(match,sizeof(messageMatch));
    if(result == -1){
        log(MATCH_MANAGER_NAME + " : No se pudo realizar la escritura en el fifo ", __FILE__, __LINE__, ERROR);
    }else if (result != sizeof(messageMatch)){
        log(MATCH_MANAGER_NAME + " : Se ha escrito una cantidad erronea de bytes en el fifo ", __FILE__, __LINE__, ERROR);
    }else{
        log("MatchManager : Se escribio un match Correctamente ", INFORMATION);
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