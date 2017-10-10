#include "MatchManager.h"

MatchManager::MatchManager() {
    this->channelToReadTeams = new FifoLectura(FIFO_READ_TEAM_OF_TEAMMANAGER);
    this->channelToWriteMatches = new FifoEscritura(FIFO_WRITE_MATCH_TO_MATCHES);
}

void MatchManager::execute() {
    this->channelToReadTeams->abrir();


    struct messageTeam* team1 = NULL;
    struct messageTeam* team2 = NULL;



    while (!this->finalize) {
/*        struct messageTeam* team1 = this->readTeam();
        struct messageTeam* team2 = this->readTeam();
        struct messageMatch *match = this->makeMatch(team1, team2);
        delete team1;
        delete team2;
        //this->writeMatch(match);
        delete match;
*/

        //agregado momentaneamente solo para finalizar felizmente
        struct messageTeam* team = this->readTeam();

        if(team->idPlayer1 == -1){
            this->finalize = true;
        }else{
            if(team1 == NULL){
                team1 = team;
            }else{
                team2 = team;
                struct messageMatch *match = this->makeMatch(team1, team2);
                delete team1;
                delete team2;
                //this->writeMatch();
                delete match;
                team1 = NULL;
                team2 = NULL;
            }
        }      

    }
    log("MatchManager ha finalizado correctamente",INFORMATION);
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
	}else if (result != sizeof(messagePlayer)){
		log(MATCH_MANAGER_NAME + " : Se ha leido una cantidad erronea de bytes del fifo ",__FILE__, __LINE__, ERROR);
    }
    
    return buff;
}

MatchManager::~MatchManager() {
    this->channelToReadTeams->cerrar();
    //this->channelToWriteMatches->cerrar();    
    delete this->channelToReadTeams;
    delete this->channelToWriteMatches;
}