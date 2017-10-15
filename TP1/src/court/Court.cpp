#include "court/Court.h"

Court::Court() {
	log("Court: Inicializando.", 3);
	log("Court: Abriendo /tmp/fifoMatches.", 3);
	this->fifoMatches = new FifoLecturaBloqueante("/tmp/fifoMatches");
	this->fifoMatches->abrir();
	log("Court: /tmp/fifoResultManager abierto.", 3);
	log("Court: Abriendo /tmp/fifoResultManager.", 3);
	this->fifoResults = new FifoEscritura("/tmp/fifoResultManager");
	this->fifoResults->abrir();
	log("Court: /tmp/fifoResultManager abierto.", 3);
	log("Court: Abriendo /tmp/fifoPlayerManager.", 3);
	this->fifoPlayerManager = new FifoEscritura("/tmp/fifoPlayerManager");
	this->fifoPlayerManager->abrir();
	log("Court: /tmp/fifoResultManager abierto.", 3);
	this->matchShouldBeCancelled = false;
	log("Court: Fin Inicializacion.", 3);
};

Court::~Court() {
	log("Court: Liberando recursos.", 3);
	log("Court: Cerrando /tmp/fifoMatches.", 3);
	this->fifoMatches->cerrar();
	log("Court: /tmp/fifoResultManager cerrado.", 3);
	log("Court: Cerrando /tmp/fifoResultManager.", 3);
	this->fifoResults->cerrar();
	log("Court: /tmp/fifoResultManager cerrado.", 3);
	log("Court: Cerrando /tmp/fifoResultManager.", 3);
	this->fifoPlayerManager->cerrar();
	log("Court: /tmp/fifoResultManager cerrado.", 3);
	delete(this->fifoMatches);
	delete(this->fifoResults);
	delete(this->fifoPlayerManager);
	log("Court: Recursos liberados.", 3);
};

void Court::runUntilThereAreNoMatchesLeft() {
	log("Court: Se abrio la cancha", 3);
	bool moreMatchesToPlay = true;
	Message* readMessage;
	while(moreMatchesToPlay) {
		log("Court: Esperando nuevo juego.", 3);
		readMessage = this->getMessage();
		moreMatchesToPlay = this->processMessage(readMessage);
		delete(readMessage);
	}
	log("Court: Cerrando cancha", 3);
};

Message* Court::getMessage() {
	struct messageMatch readMessage;
	int readBytes = this->fifoMatches->leer(static_cast<void*>(&readMessage), sizeof(readMessage));
	if (readBytes < 0) {
		log("Court: ** Error ** En la lectura. Errno: ", strerror(errno), 1);
	} else if (readBytes == 0) {
		log("Court: ** Warning ** Se leyeron 0 bytes. Ignorando.", 2);
	} else {
		log("Court: Mensaje leido correctamente. Cantidad de bytes: ", readBytes, 3);
		return (new Message(readMessage));
	}
	return (new Message());
};

bool Court::processMessage(Message* message) {
	bool moreMatchesToPlay = false;
	int operation = message->getOperation();
	log("Court: Se recibio un mensaje. Operacion: ", operation, 3);
	if (operation == PLAY) {
		log("Court: Se recibio el mensaje de nuevo juego.", 3);
		this->playGame(message);
		moreMatchesToPlay = true;
	} else if (operation == CLOSE) {
		log("Court: Se recibio el mensaje de cerrado de cancha.", 3);
		moreMatchesToPlay = false;
	}
	return moreMatchesToPlay;
};

void Court::playGame(Message* message) {
	Match* match = this->getMatchFromMessage(message);
	log("Court: Conzando a jugar el partido.", 3);
	match->play();
	log("Court: Partido finalizado. Resultado: ", match, 3);
	this->sendMessages(match);
	delete(match);
};

Match* Court::getMatchFromMessage(Message* message) {
	Team* firstTeam = new Team(
		message->getFirstTeamFirstPlayer(),
		message->getFirstTeamSecondPlayer()
	);

	Team* secondTeam = new Team(
		message->getSecondTeamFirstPlayer(),
		message->getSecondTeamSecondPlayer()
	);
	return (new Match(firstTeam, secondTeam));
};

void Court::sendMessages(Match* match) {
	if (!match->wasMatchCancelled()) {
		log("Court: Enviando mensajes de resultados.", 3);
		struct messageResult resultMessage = match->getResultMessage();
		this->fifoResults->escribir(static_cast<void*>(&resultMessage), sizeof(resultMessage));
	}
	log("Court: Enviando mensajes de estado de finalizacion del partido.", 3);
	std::vector<messagePlayer> matchStateMessages = match->getResultMessages();
	for(messagePlayer matchStateMessage : matchStateMessages) {
		this->fifoPlayerManager->escribir(static_cast<void*>(&matchStateMessage), sizeof(matchStateMessage));
	}
};


