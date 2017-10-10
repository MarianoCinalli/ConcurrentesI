#include "TeamManager.h"
#include <string.h>

TeamManager::TeamManager() {
    this->players = new std::vector<int>();
    this->playsByPlayer = new std::map<int, std::vector<int>*>();
    this->channelToRead = new FifoLectura(FILE_FIFO_READ);
    this->channelToWrite = new FifoEscritura(FILE_FIFO_WRITE);
    //this->fifoTeamManager->abrir();
}

void TeamManager::execute() {
    struct messageTeam* team;
    struct messagePlayer* message;

    while (!this->finalize) {
        message = this->readPlayer();
        this->parseMessage(message);
        team = this->makeTeam();
        this->writeTeam(team);
    }
}

void TeamManager::parseMessage(struct messagePlayer* message){
    switch (message->status){
        
        case CommandType::killType :
            this->finalize = true;
            break;

        case CommandType::gameCanceled :
            this->cancelLastGameOfPLayer(message->idPlayer);
            break;

        default :
            //se agrega un player para formar equipo
            this->addPlayer(message->idPlayer);
    }
    
    delete message;
}


void TeamManager::addPlayer(int idPlayer){
    std::vector<int>::iterator it = std::find(this->players->begin(), this->players->end(), idPlayer);
    if (it != this->players->end()){
        log(TEAM_MANAGER_NAME + "se recibe un jugador que ya existe para formar equipo, jugador con id: ", idPlayer, ERROR);
    }else{
        this->players->push_back(idPlayer);
    }    
}

/*
* Forma un team, verificando que no se repitan las parejas
*/
struct messageTeam* TeamManager::makeTeam(){
    int idPlayer1;
    int idPlayer2;
    struct messageTeam* team;

    for(unsigned i = 0; i < this->players->size();i++){
        for(unsigned j = i+1; j < this->players->size();j++){
            idPlayer1 = this->players->at(i);
            idPlayer2 = this->players->at(j);
            if(!this->playersPlayBetween(idPlayer1,idPlayer2)){
                // falta remover jugadores
                //
                //
                //
                team = new messageTeam;
                team->idPlayer1 = idPlayer1;
                team->idPlayer2 = idPlayer2;
                return team;
            }
        }
    }
    log(TEAM_MANAGER_NAME + "no se pudo formar equipo",INFORMATION);
    return NULL;
}

void TeamManager::cancelLastGameOfPLayer(int idPlayer){
    
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

struct messagePlayer* TeamManager::readPlayer(){
	struct messagePlayer *buff = new messagePlayer;
	memset(buff,0,sizeof(messagePlayer));

    int result = this->channelToRead->leer(buff,sizeof(messagePlayer));
    
	if(result == -1){
		log(TEAM_MANAGER_NAME + "No se pudo realizar la lectura del fifo ","sss",100, 1);
	}else if (result != sizeof(messagePlayer)){
		log(TEAM_MANAGER_NAME + "Se ha leido una cantidad erronea de bytes del fifo ","sss",100, 1);
    }
    
    return buff;
}


void TeamManager::writeTeam(struct messageTeam * team){
			
    int result = this->channelToWrite->escribir(team,sizeof(messageTeam));

    if(result == -1){
        log(TEAM_MANAGER_NAME + "No se pudo realizar la escritura en el fifo ", __FILE__, __LINE__, ERROR);
    }else if (result != sizeof(messageTeam)){
        log(TEAM_MANAGER_NAME + "Se ha escrito una cantidad erronea de bytes en el fifo ", __FILE__, __LINE__, ERROR);
    }

    delete team;
}


TeamManager::~TeamManager() {
    std::map<int, std::vector<int>*>::iterator it;
    for(it = this->playsByPlayer->begin(); it != this->playsByPlayer->end(); it++) {
        std::vector<int> *plays = (*it).second;
        delete plays; 
    }
    delete this->playsByPlayer;
    this->channelToRead->cerrar();
    this->channelToWrite->cerrar();    
    delete this->channelToRead;
    delete this->channelToWrite;
}