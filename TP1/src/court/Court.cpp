#include "court/Court.h"

Court::Court() {
	this->fifoMatches = new FifoLectura(FIFO_READ_MATCH_OF_MATCHMANAGER);	
	this->fifoResults = new FifoEscritura(FIFO_WRITE_RESULT_TO_RESULTMANAGER);
	this->fifoPlayerManager = new FifoEscritura(FIFO_WRITE_STATUS_TO_PLAYERMANAGER);
	this->matchShouldBeCancelled = false;
};

Court::~Court() {
	this->fifoMatches->cerrar();
	this->fifoMatches->eliminar();
	this->fifoResults->cerrar();
	this->fifoPlayerManager->cerrar();
	this->fifoPlayerManager->eliminar();
	delete(this->fifoMatches);
	delete(this->fifoResults);
	delete(this->fifoPlayerManager);
};

void Court::runUntilThereAreNoMatchesLeft() {
	this->fifoMatches->abrir();
	this->fifoResults->abrir();
	this->fifoPlayerManager->abrir();
	log("Se abrio la cancha", 3);
	bool moreMatchesToPlay = true;
	Message* readMessage;
	while(moreMatchesToPlay) {
		log("Esperando nuevo juego.", 3);
		readMessage = this->getMessage();
		moreMatchesToPlay = this->processMessage(readMessage);
		delete(readMessage);
	}
	log("Cerrando cancha", 3);
};

Message* Court::getMessage() {
	struct messageMatch readMessage;
	int readBytes = this->fifoMatches->leer(static_cast<void*>(&readMessage), sizeof(readMessage));
	if (readBytes < 0) {
		log("** Error ** En la lectura. Errno: ", strerror(errno), 1);
	} else if (readBytes == 0) {
		log("** Warning ** Se leyeron 0 bytes. Ignorando.", 2);
	} else {
		log("Mensaje leido correctamente. Cantidad de bytes: ", readBytes, 3);
		return (new Message(readMessage));
	}
	return (new Message());
}

bool Court::processMessage(Message* message) {
	bool moreMatchesToPlay = false;
	int operation = message->getOperation();
	log("Se recibio un mensaje. Operacion: ", operation, 3);
	if (operation == PLAY) {
		log("Se recibio el mensaje de nuevo juego.", 3);
		this->playGame(message);
		moreMatchesToPlay = true;
	} else if (operation == CLOSE) {
		log("Se recibio el mensaje de cerrado de cancha.", 3);
		moreMatchesToPlay = false;
	}
	return moreMatchesToPlay;
}

void Court::playGame(Message* message) {
	Team* firstTeam = new Team(
		message->getFirstTeamFirstPlayer(),
		message->getFirstTeamSecondPlayer()
	);

	Team* secondTeam = new Team(
		message->getSecondTeamFirstPlayer(),
		message->getSecondTeamSecondPlayer()
	);
	Match* match = new Match(firstTeam, secondTeam);
	log("Conzando a jugar el partido.", 3);
	match->play();
	if (this->matchShouldBeCancelled) {
		match->cancelMatch();
	}
	log("Partido finalizado. Resultado: ", match, 3);
	this->sendMessages(match);
	delete(match);
}

void Court::sendMessages(Match* match) {
	log("Enviando mensajes de resultados.", 3);
	struct messageResult resultMessage = match->getResultMessage();
	this->fifoResults->escribir(static_cast<void*>(&resultMessage), sizeof(resultMessage));
	log("Enviando mensajes de estado de finalizacion del partido.", 3);
	std::vector<messagePlayer> matchStateMessages = match->getResultMessages();
	for(messagePlayer matchStateMessage : matchStateMessages) {
		this->fifoPlayerManager->escribir(static_cast<void*>(&matchStateMessage), sizeof(matchStateMessage));
	}
}
