#include "../headers/playerResult/ResultManager.h"
#include<string>

ResultManager::ResultManager() {
    this->finalizedProcess = false;
    this->playerResults = new std::map<int, PlayerResult*>();
    this->channelToRead = new FifoLectura(FIFO_READ_RESULT_TO_RESULTMANAGER);
    this->channelToRead->abrir();
    log(RESULT_MANAGER_NAME + " : Se abrio FIFO de lectura " + FIFO_READ_RESULT_TO_RESULTMANAGER,INFORMATION);
    this->maxScore = -1;
    this->winners = new std::vector<PlayerResult*>();
	log(RESULT_MANAGER_NAME + " : Se construyo FIFO de lectura " + FIFO_READ_RESULT_TO_RESULTMANAGER, INFORMATION);
}

void ResultManager::execute() {

    while (!this->finalizedProcess) {
        struct messageResult *result = readFifoResultManager();
        this->parseMessage(result);
        delete result;
    }
    this->showWinners();
}

void ResultManager::parseMessage(struct messageResult *result){
    switch(result->operation){
        case ResultCommands::RESULT:
            this->proccesResult(result);
            break;
        
        case ResultCommands::EXIT:
            log(RESULT_MANAGER_NAME + " : Llego mensaje de fin de partido",INFORMATION);
            this->finalizedProcess = true;
            break;
    }
}

void ResultManager::proccesResult(struct messageResult *result){
    bool case1ToTeam1 = (result->setsWonTeam1 == 3) && ((result->setsWonTeam2 == 0) || (result->setsWonTeam2 == 1));
    bool case2ToTeam1 = (result->setsWonTeam1 == 3) && (result->setsWonTeam2 == 2);
    bool case1ToTeam2 = (result->setsWonTeam2 == 3) && ((result->setsWonTeam1 == 0) || (result->setsWonTeam1 == 1));
    bool case2ToTeam2 = (result->setsWonTeam2 == 3) && (result->setsWonTeam1 == 2);

    PlayerResult *player1Team1Result = searchPlayerResult(result->idPlayer1_Team1);
    PlayerResult *player2Team1Result = searchPlayerResult(result->idPlayer2_team1);
    PlayerResult *player1Team2Result = searchPlayerResult(result->idPlayer1_Team2);
    PlayerResult *player2Team2Result = searchPlayerResult(result->idPlayer2_team2);
    if (case1ToTeam1) {
        log(RESULT_MANAGER_NAME + " : Equipo 1 gano por caso uno", INFORMATION);
        player1Team1Result->addScore(THREE);
        player2Team1Result->addScore(THREE);
        player1Team2Result->addScore(ZERO);
        player2Team2Result->addScore(ZERO);
    } else if (case2ToTeam1) {
        log(RESULT_MANAGER_NAME + " : Equipo 1 gano por caso dos", INFORMATION);
        player1Team1Result->addScore(TWO);
        player2Team1Result->addScore(TWO);
        player1Team2Result->addScore(ONE);
        player2Team2Result->addScore(ONE);
    } else if (case1ToTeam2) {
        log(RESULT_MANAGER_NAME + " : Equipo 2 gano por caso uno", INFORMATION);
        player1Team1Result->addScore(ZERO);
        player2Team1Result->addScore(ZERO);
        player1Team2Result->addScore(THREE);
        player2Team2Result->addScore(THREE);
    } else if (case2ToTeam2) {
        log(RESULT_MANAGER_NAME + " : Equipo 2 gano por caso dos", INFORMATION);
        player1Team1Result->addScore(ONE);
        player2Team1Result->addScore(ONE);
        player1Team2Result->addScore(TWO);
        player2Team2Result->addScore(TWO);
    }

    managerWinners(player1Team1Result, player2Team1Result, player1Team2Result, player2Team2Result);
    
}



void ResultManager::managerWinners(PlayerResult *player1Team1Result, PlayerResult *player2Team1Result, PlayerResult *player1Team2Result, PlayerResult *player2Team2Result) {
    this->managerWinner(player1Team1Result);
    this->managerWinner(player2Team1Result);
    this->managerWinner(player1Team2Result);
    this->managerWinner(player2Team2Result);
}

void ResultManager::managerWinner(PlayerResult *playerResult) {
    if (playerResult->getScore() > this->maxScore) {
        this->maxScore = playerResult->getScore();
        this->winners->clear();
        this->winners->push_back(playerResult);
    } else if (playerResult->getScore() == this->maxScore) {
        std::vector<PlayerResult*>::iterator it = std::find(this->winners->begin(), this->winners->end(), playerResult);
        bool alreadyExitsWinner = it != this->winners->end();
        if (!alreadyExitsWinner) {
            this->winners->push_back(playerResult);
        }
    }
    playerResult->showScore();
}

void ResultManager::showWinners() {
    log(RESULT_MANAGER_NAME + " : Se muestra al o a los ganadores ", __FILE__, __LINE__, ERROR);
    std::vector<PlayerResult*>::iterator it;
    for(it = this->winners->begin(); it != this->winners->end(); it++) {
        PlayerResult *playerResult = *it;
        std::cout<< "Ganador jugador " + std::to_string(playerResult->getId()) + " con puntaje " + std::to_string(playerResult->getScore())<<std::endl;
        log(RESULT_MANAGER_NAME + " : Ganador jugador " + std::to_string(playerResult->getId()) + " con puntaje " + std::to_string(playerResult->getScore()), INFORMATION);
    }
}

PlayerResult* ResultManager::searchPlayerResult(int idPlayer) {
    std::map<int, PlayerResult*>::iterator it = this->playerResults->find(idPlayer);
    if (it != this->playerResults->end()) {
        log(RESULT_MANAGER_NAME + " : Se encontro al playerResult " + std::to_string(idPlayer), __FILE__, __LINE__, INFORMATION);
        PlayerResult *playerResult = it->second;
        playerResult->showScore();
        return it->second;
    }
    log(RESULT_MANAGER_NAME + " : No se encontro al playerResult " + std::to_string(idPlayer), __FILE__, __LINE__, WARNING);
    PlayerResult *playerResult = new PlayerResult(idPlayer);
    this->playerResults->insert(std::pair<int,PlayerResult*>(idPlayer,playerResult));
    return searchPlayerResult(idPlayer);
}

struct messageResult* ResultManager::readFifoResultManager() {
    struct messageResult *buff = new messageResult;
	memset(buff,0,sizeof(messageResult));
	int result = this->channelToRead->leer(buff,sizeof(messageResult));
	if(result == -1){
        log(RESULT_MANAGER_NAME + " : **Error** No se pudo realizar la lectura del fifo ",__FILE__, __LINE__, ERROR);
        exit(1);
	}if(result == 0){
        log(RESULT_MANAGER_NAME + " : El fifo para lectura se ha cerrado, finalizando proceso",__FILE__, __LINE__, INFORMATION);
        this->finalizedProcess = true;
	} else if (result != sizeof(messageResult)){
        log(RESULT_MANAGER_NAME + " : **Error** Se ha leido una cantidad erronea de bytes del fifo ", __FILE__, __LINE__, ERROR);
        exit(1);
	}
	
	return buff;
}

ResultManager::~ResultManager() {
    log(RESULT_MANAGER_NAME + " Se liberan recursos ", INFORMATION);
    std::map<int, PlayerResult*>::iterator it;
    for(it = this->playerResults->begin(); it != this->playerResults->end(); it++) {
        PlayerResult *aPlayerResult = it->second;
        delete aPlayerResult;
    }
    delete this->playerResults;
    delete this->winners;
    delete this->channelToRead;
}