#include "TeamManager.h"
#include <string.h>

TeamManager::TeamManager() {
    this->players = new std::vector<int>();
    this->playsByPlayer = new std::map<int, std::vector<int>*>();
    this->channelToRead = new FifoLectura(FIFO_READ_TEAM_OF_PLAYER);
    //this->channelToWrite = new FifoEscritura(FIFO_WRITE_TEAM_TO_MATCH);
    //this->fifoTeamManager->abrir();
}

void TeamManager::execute() {
    struct messageTeam* team;
    struct messagePlayer* message;
    this->channelToRead->abrir();

    while (!this->finalize) {
        message = this->readPlayer();
        this->parseMessage(message);
        team = this->makeTeam();
        //this->writeTeam(team);
    }
    log("El proceso TeamManager finaliza correctamente ",INFORMATION);
    log("la jugadores para formar equipos",this->players->size(),INFORMATION);
}

void TeamManager::parseMessage(struct messagePlayer* message){
    switch (message->status){
        
        case CommandType::killType :
            this->finalize = true;
            std::cout<<"llega kill"<<std::endl;
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
        log("se recibe un jugador que ya existe en espera para formar equipo, jugador con id: ", idPlayer, ERROR);
    }else{
        this->players->push_back(idPlayer);
        try{
            playsByPlayer->at(idPlayer);

        }catch(std::out_of_range e){
            //si no existe se agrega
            std::vector<int>* playMates = new std::vector<int>(); 
            (*playsByPlayer)[idPlayer] = playMates;
        }
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
            if(this->canPlayersFormTeam(idPlayer1,idPlayer2)){
                team = this->formTeam(idPlayer1,idPlayer2);
                this->removePlayer(idPlayer1);
                this->removePlayer(idPlayer2);
                log("se formo un equipo",INFORMATION);
                return team;
            }
        }
    }
    log("NO se formo un equipo",INFORMATION);
    return NULL;
}

void TeamManager::cancelLastGameOfPLayer(int idPlayer){
    
}

struct messageTeam* TeamManager::formTeam(int idPlayer1, int idPlayer2) {

    struct messageTeam* team = NULL;

    try{
        std::vector<int> *playMates1 = this->playsByPlayer->at(idPlayer1);
        std::vector<int> *playMates2 = this->playsByPlayer->at(idPlayer2);

        //actualizacion de registro de compañeros
        playMates1->push_back(idPlayer2);
        playMates2->push_back(idPlayer1);

        team = new messageTeam;
        team->idPlayer1 = idPlayer1;
        team->idPlayer2 = idPlayer2;

    }catch(std::out_of_range e){

        log("error al actualizar registro de equipos, jugador id: ",idPlayer1 ,ERROR);
        log("error al actualizar registro de equipos, jugador id: ",idPlayer2 ,ERROR);
    }

    return team;
}


void TeamManager::removePlayer(int idPlayer){
    std::vector<int>::iterator it = std::find(this->players->begin(), players->end(), idPlayer);
    if (it != players->end())
        this->players->erase(it);
    else{
        log("no se pueden remover jugadores luego de crear un equipo",idPlayer,ERROR);
        exit(1);
    }
}




bool TeamManager::canPlayersFormTeam(int idPlayer1, int idPlayer2) {

    try{
        std::vector<int> *plays = this->playsByPlayer->at(idPlayer1);
        std::vector<int>::iterator it = std::find(plays->begin(), plays->end(), idPlayer2);

        if (it != plays->end())
          //si ya esta asociado a su lista no se puede
          return false;
        else
          //si no esta asociado pueden formar equipo  
          return true;
    }catch(std::out_of_range e){
        log("error al buscar registro de equipos, jugador id: ",idPlayer1 ,ERROR);
        return false;  
    }  
}

struct messagePlayer* TeamManager::readPlayer(){
	struct messagePlayer *buff = new messagePlayer;
	memset(buff,0,sizeof(messagePlayer));

    int result = this->channelToRead->leer(buff,sizeof(messagePlayer));
    
	if(result == -1){
		log("No se pudo realizar la lectura del fifo ","sss",100, 1);
	}else if (result != sizeof(messagePlayer)){
		log("Se ha leido una cantidad erronea de bytes del fifo ","sss",100, 1);
    }
    
    return buff;
}


void TeamManager::writeTeam(struct messageTeam * team){
			
    int result = this->channelToWrite->escribir(team,sizeof(messageTeam));

    if(result == -1){
        log("No se pudo realizar la escritura en el fifo ", __FILE__, __LINE__, ERROR);
    }else if (result != sizeof(messageTeam)){
        log("Se ha escrito una cantidad erronea de bytes en el fifo ", __FILE__, __LINE__, ERROR);
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