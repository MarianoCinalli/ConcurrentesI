#include "TeamManager.h"
#include <string.h>

TeamManager::TeamManager() {
    this->players = new std::vector<int>();
    this->playsByPlayer = new std::map<int, std::vector<int>*>();
    this->channelToRead = new FifoLectura(FIFO_READ_PLAYER_OF_PLAYERMANAGER);
    this->channelToWrite = new FifoEscritura(FIFO_WRITE_TEAM_TO_MATCHMANAGER);
    this->channelToWritePlayerManager = new FifoEscritura(FIFO_READ_COMMAND_OF_COMMANDMANAGER);    
    this->channelToRead->abrir();
    this->channelToWrite->abrir();
    this->channelToWritePlayerManager->abrir();
    log(TEAM_MANAGER_NAME + " : Se abrio FIFO de lectura " + FIFO_READ_PLAYER_OF_PLAYERMANAGER,INFORMATION);
    log(TEAM_MANAGER_NAME + " : Se abrio FIFO de escritura " + FIFO_WRITE_TEAM_TO_MATCHMANAGER,INFORMATION);
}

void TeamManager::execute() {
    struct messagePlayer* message;

    while (!this->finalize) {
        message = this->readPlayer();
        this->parseMessage(message);
        delete message;
    }
    log(TEAM_MANAGER_NAME + " : El proceso finaliza correctamente ",INFORMATION);
    log(TEAM_MANAGER_NAME + " : La jugadores que quedaron para formar equipos",this->players->size(),INFORMATION);
}

void TeamManager::parseMessage(struct messagePlayer* message){
    switch (message->status){
        
        case CommandType::killType :{
            this->finalize = true;
            struct messageTeam *team = new messageTeam;
            team->operation = CLOSE;
            this->writeTeam(team);
            delete team;
            break;
        }

        case CommandType::gameCanceled :
            this->cancelLastGameOfPLayer(message->idPlayer);
            break;


        case CommandType::readyToTeam :
            this->formAllTeams();
            break;

        case CommandType::waitToTeam :
            //se agrega un player para formar equipo
            this->addPlayer(message->idPlayer);
    }
}

void TeamManager::formAllTeams(){
    log(TEAM_MANAGER_NAME + " : se recibio el Flag y se formaran los teams",INFORMATION);
    std::vector<messageTeam*> teams;
    struct messageTeam* team;
    while ((team = this->makeTeam()) != NULL){
        teams.push_back(team);
    }

    std::vector<messageTeam*>::iterator it;
    for(it = teams.begin();it != teams.end();it++){
        this->writeTeam(*it);
    }

    //escribo a PlayerManager la cantidad de equipos creados
    messagePlayer message;
    message.idPlayer = teams.size();
    message.status = CommandType::amountTeams;
    int result = this->channelToWritePlayerManager->escribir(&message,sizeof(messagePlayer));

    if(result == -1){
        log(TEAM_MANAGER_NAME + " No se pudo realizar la escritura de la cantidad de equipos Formados ", __FILE__, __LINE__, ERROR);
        exit(1);
    }else if (result != sizeof(messagePlayer)){
        log(TEAM_MANAGER_NAME + " Se ha escrito de forma erronea la cantidad de equipos Formados  ", __FILE__, __LINE__, ERROR);
        exit(1);
    }else{
        log(TEAM_MANAGER_NAME + " Se ha escrito la cantidad de equipos Formados: ",teams.size(), INFORMATION);        
    }

    for(it = teams.begin();it != teams.end();it++){
        delete (*it);        
    }
    teams.clear();
}

void TeamManager::addPlayer(int idPlayer){
    std::vector<int>::iterator it = std::find(this->players->begin(), this->players->end(), idPlayer);
    if (it != this->players->end()){
        log(TEAM_MANAGER_NAME + "se recibe un jugador que ya existe para formar equipo, jugador con id: ", idPlayer, ERROR);
        exit(1);
    }else{
        //se acola para formar equipos
        this->players->push_back(idPlayer);
        log(TEAM_MANAGER_NAME + "se recibe un jugador para formar equipo, jugador con id: ", idPlayer, INFORMATION);        
        try{
            playsByPlayer->at(idPlayer);

        }catch(std::out_of_range e){
            //si no existe resgistro del jugador se agrega su id
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

                std::string messageLog =" IdJugador1: "+std::to_string(idPlayer1) + ", IdJugador2: "+std::to_string(idPlayer2);
                log(TEAM_MANAGER_NAME + " : Se formo un equipo, " + messageLog,INFORMATION);
                return team;
            }
        }
    }
    return NULL;
}



/**
 * Al recibir un mensaje de cancelacion de juego, se elimina el ultimo compañero
 * registrado, por lo tanto debe tener al menos un compañero ya registrado 
 **/
void TeamManager::cancelLastGameOfPLayer(int idPlayer){

    try{

        std::vector<int>* playMates = playsByPlayer->at(idPlayer);

        if (!playMates->empty()){
            int playMate = playMates->back();
            playMates->pop_back();
            std::string team = std::to_string(idPlayer) +  " - " + std::to_string(playMate);
            log(TEAM_MANAGER_NAME + " : Se elimino el registro de equipo formado por: " + team,INFORMATION);
            
        }else{
            log(TEAM_MANAGER_NAME + " : No existe registro a cancelar del ultimo compañero del jugador con id: ",idPlayer,ERROR);
            exit(1);
        }

    }catch(std::out_of_range e){
        log(TEAM_MANAGER_NAME + " : No existe registro del jugador con id: ",idPlayer,ERROR);
        exit(1);
    }
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
        team->operation = PLAY;

    }catch(std::out_of_range e){

        log(TEAM_MANAGER_NAME + " : **Error** al actualizar registro de equipos, jugador id: ",idPlayer1 ,ERROR);
        log(TEAM_MANAGER_NAME + " : **Error** al actualizar registro de equipos, jugador id: ",idPlayer2 ,ERROR);
        exit(1);
    }

    return team;
}


void TeamManager::removePlayer(int idPlayer){
    std::vector<int>::iterator it = std::find(this->players->begin(), players->end(), idPlayer);
    if (it != players->end())
        this->players->erase(it);
    else{
        log(TEAM_MANAGER_NAME + " : No se puedo remover a un jugador luego de crear un equipo, jugador con id",idPlayer,ERROR);
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
        log(TEAM_MANAGER_NAME + " : Error al buscar registro de equipos, jugador id: ",idPlayer1 ,ERROR);
        exit(1); 
    }  
}

struct messagePlayer* TeamManager::readPlayer(){
	struct messagePlayer *buff = new messagePlayer;
	memset(buff,0,sizeof(messagePlayer));

    int result = this->channelToRead->leer(buff,sizeof(messagePlayer));
    
	if(result == -1){
		log(TEAM_MANAGER_NAME + " : No se pudo realizar la lectura del fifo ", __FILE__, __LINE__, ERROR);
	}else if(result == 0){
        log(TEAM_MANAGER_NAME + " : No se pudo realizar la lectura del fifo, porque se ha cerrado el fifo ", __FILE__, __LINE__, WARNING);        
        this->finalize = true;
    }else if (result != sizeof(messagePlayer)){
		log(TEAM_MANAGER_NAME + " : Se ha leido una cantidad erronea de bytes del fifo ", __FILE__, __LINE__, ERROR);
    }
    
    return buff;
}


void TeamManager::writeTeam(struct messageTeam * team){

    int result = this->channelToWrite->escribir(team,sizeof(messageTeam));

    if(result == -1){
        log(TEAM_MANAGER_NAME + " No se pudo realizar la escritura en el fifo ", __FILE__, __LINE__, ERROR);
        exit(1);
    }else if (result != sizeof(messageTeam)){
        log(TEAM_MANAGER_NAME + " Se ha escrito una cantidad erronea de bytes en el fifo ", __FILE__, __LINE__, ERROR);
        exit(1);
    }
}


TeamManager::~TeamManager() {
    std::map<int, std::vector<int>*>::iterator it;
    for(it = this->playsByPlayer->begin(); it != this->playsByPlayer->end(); it++) {
        std::vector<int> *plays = (*it).second;
        delete plays; 
    }
    delete this->playsByPlayer;
    delete this->players;
    this->channelToRead->cerrar();
    this->channelToWrite->cerrar();  
    delete this->channelToRead;
    delete this->channelToWrite;

    this->channelToWritePlayerManager->cerrar();
    delete this->channelToWritePlayerManager;

}