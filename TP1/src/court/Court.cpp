#include "court/Court.h"

Court::Court() {
	this->fifoMatches = new FifoLectura("/tmp/fifoMatches");	
	this->fifoMatches->abrir();
	this->fifoResults = new FifoEscritura("/tmp/fifoResultManager");
	this->fifoResults->abrir();
	this->fifoPlayerManager = new FifoEscritura("/tmp/fifoPlayerManager");
	this->fifoPlayerManager->abrir();
	this->registerHandler();
};

Court::~Court() {
	this->fifoMatches->cerrar();
	this->fifoResults->cerrar();
	this->fifoPlayerManager->cerrar();
	delete(this->fifoMatches);
	delete(this->fifoResults);
	delete(this->fifoPlayerManager);
};

void Court::runUntilThereAreNoMatchesLeft() {
	log("Se abrio la cancha", 3);
	bool moreMatchesToPlay = true;
	while(moreMatchesToPlay) {
		log("Esperando nuevo juego.", 3);
		readMessage = this->getMessage();
		moreMatchesToPlay = this->processMessage(readMessage);
		delete(readMessage);
	}
	log("Cerrando cancha", 3);
};

Message* Court::getMessage() {
	int readBytes = this->fifoMatches->leer(static_cast<void*>(&this->readMessage), sizeof(this->readMessage));
	if (readBytes < 0) {
		log("** Error ** En la lectura. Errno: ", strerror(errno), 1);
	} else if (readBytes == 0) {
		log("** Warning ** Se leyeron 0 bytes. Ignorando.", 2);
	} else {
		log("Mensaje leido correctamente. Cantidad de bytes: ", readBytes, 3);
		return (new Message(this->readMessage));
	}
	return (new Message());
};

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
};

void Court::playGame(Message* message) {
	Match* match = this->getMatchFromMessage(message);
	log("Conzando a jugar el partido.", 3);
	match->play();
	log("Partido finalizado. Resultado: ", match, 3);
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
	if (!match->wasCancelled()) {
		log("Enviando mensajes de resultados.", 3);
		struct messageResult resultMessage = match->getResultMessage();
		this->fifoResults->escribir(static_cast<void*>(&resultMessage), sizeof(resultMessage));
	}
	log("Enviando mensajes de estado de finalizacion del partido.", 3);
	std::vector<messagePlayer> matchStateMessages = match->getResultMessages();
	for(messagePlayer matchStateMessage : matchStateMessages) {
		this->fifoPlayerManager->escribir(static_cast<void*>(&matchStateMessage), sizeof(matchStateMessage));
	}
};

void Court::handler_cleanAndExit() {
	Message* message = new Message(this->readMessage);
	if (message->getOperation()) {
		Match* match = this->getMatchFromMessage(message);
		match->cancelMatch();
		this->sendMessages(match);
	}
	exit(EXIT_SUCCESS);
};

void Court::registerHandler() {
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = handler_cleanAndExit;
	sigemptyset (&sa.sa_mask);
	sigaddset (&sa.sa_mask, signum);
	sigaction (signum, &sa, 0);
};
