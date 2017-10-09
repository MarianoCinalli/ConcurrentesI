#include "court/Court.h"

Court::Court() {
	this->fifoMatches = new FifoLectura("/tmp/fifoMatches");	
	this->fifoMatches->abrir();
	this->fifoResults = new FifoEscritura("/tmp/fifoResultManager");
	this->fifoResults->abrir();
};

Court::~Court() {
	this->fifoMatches->cerrar();
	this->fifoResults->cerrar();
	delete(this->fifoMatches);
	delete(this->fifoResults);
};

void Court::runUntilThereAreNoMatchesLeft() {
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
	if (operation == 0) {
		log("Se recibio el mensaje de nuevo juego.", 3);
		this->playGame(message);
		moreMatchesToPlay = true;
	} else if (operation == 1) {
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
	log("Partido finalizado. Resultado: ", match, 3);
	struct messageResult toWriteMessage = match->getResultMessage();
	this->fifoResults->escribir(static_cast<void*>(&toWriteMessage), sizeof(toWriteMessage));;
	delete(match);
}
