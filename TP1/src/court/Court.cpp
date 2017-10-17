#include "court/Court.h"
#include "court/signal/SIGINT_Handler.h"

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
	this->readMessage = (const struct messageMatch){ IGNORE };
	this->match = NULL;
	this->moreMatchesToPlay = true;
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
	log("Court: Registrando SIGINT handler.", 3);
	SIGINT_Handler sigint_handler(this);
	SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);
	log("Court: Se abrio la cancha", 3);
	Message* message;
	while(this->moreMatchesToPlay || sigint_handler.getGracefulQuit()) {
		log("Court: Esperando nuevo juego.", 3);
		message = this->getMessage();
		this->moreMatchesToPlay = this->processMessage(message);		
		delete(message);
		// Reseting data ----------------------------------------------------------------
		this->readMessage = (const struct messageMatch){ IGNORE };
		Match* playedMatch = this->match; // Para no tener riezgo de un null pointer exep.
		this->match = NULL;
		delete(playedMatch);
		// Reseting data ----------------------------------------------------------------
	}
	log("Court: Cerrando cancha", 3);
};

Message* Court::getMessage() {
	int readBytes = this->fifoMatches->leer(static_cast<void*>(&(this->readMessage)), sizeof(this->readMessage));
	if (readBytes < 0) {
		log("Court: ** Error ** En la lectura. Errno: ", strerror(errno), 1);
	} else if (readBytes == 0) {
		log("Court: ** Warning ** Se leyeron 0 bytes. Ignorando.", 2);
	} else {
		log("Court: Mensaje leido correctamente. Cantidad de bytes: ", readBytes, 3);
	}
	return (new Message(this->readMessage));
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
	} else {
		log("Court: Se ignora la operacion.", 3);
		moreMatchesToPlay = true;
	}
	return moreMatchesToPlay;
};

void Court::playGame(Message* message) {
	this->match = this->getMatchFromMessage(message);
	log("Court: Conzando a jugar el partido.", 3);
	this->match->play();
	log("Court: Partido finalizado. Resultado: ", this->match, 3);
	this->sendMessages(this->match);
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
		// Solo se envian si el partido termino.
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

messageMatch Court::getReadMessage() {
	return this->readMessage;
};

Match* Court::getMatch() {
 	return this->match;
};
