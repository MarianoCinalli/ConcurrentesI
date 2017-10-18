#include "courtManager/CourtManager.h"

/* Las canchas mas cercanas a la orilla son las de [0][0] a [0][columns]
 */

// Constructor
CourtManager::CourtManager(int rows, int columns) : beach(rows, std::vector<CourtCM*>(columns)) {
	log("CourtManager: Inicializando.", 3);
	log("CourtManager: Columnas: ", columns, 3);
	log("CourtManager: Filas: ", rows, 3);
	this->rows = rows;
	this->columns = columns;
	this->tideLevel = 0;
	log("CourtManager: Abriendo fifo", 3);
//	this->fifoCourtManager = new FifoLectura("/tmp/fifoCourtManager");	
	this->fifoCourtManager = new FifoLectura(FIFO_READ_TIDE_OF_COMMANDMANAGER);	
	this->fifoCourtManager->abrir();
	log("CourtManager: Fifo abierto", 3);
	log("CourtManager: Abriendo canchas.", 3);
	this->initializeCourts();
	log("CourtManager: Canchas abiertas.", 3);
	log("CourtManager: Inicializacion finalizada.", 3);
};

void CourtManager::initializeCourts() {
	for (int rowNumber = 0; rowNumber < this->rows; ++rowNumber) {
		for (int columnNumber = 0; columnNumber < this->columns; ++columnNumber) {
			// Crea y abre la cancha
			log("CourtManager: Abriendo cancha nro.:" + std::to_string(rowNumber) + " " + std::to_string(columnNumber) , 3);
			this->beach[rowNumber][columnNumber] = new CourtCM();
		}
	}
};

void CourtManager::administrateCourts() {
	log("CourtManager: El court manager esta administrando las canchas", 3);
	bool close = false;
	MessageCM* readMessage;
	while(!close) {
		readMessage = this->getMessage();
		close = this->processMessage(readMessage);
		delete(readMessage);
	}
	log("CourtManager: Cerrando court manager", 3);
};

MessageCM* CourtManager::getMessage() {
	struct messageCourtManager readMessage;
	int readBytes = this->fifoCourtManager->leer(static_cast<void*>(&readMessage), sizeof(readMessage));
	if (readBytes < 0) {
		log("CourtManager: ** Error ** En la lectura. Errno: ", strerror(errno), 1);
	} else if (readBytes == 0) {
		log("CourtManager: ** Warning ** Se leyeron 0 bytes. Ignorando.", 2);
	} else {
		log("CourtManager: Mensaje leido correctamente. Cantidad de bytes: ", readBytes, 3);
		return (new MessageCM(readMessage));
	}
	return (new MessageCM());
};

bool CourtManager::processMessage(MessageCM* message) {
	bool shouldEnd = false;
	int operation = message->getOperation();
	log("CourtManager: Se recibio un mensaje. Operacion: ", operation, 3);
	if (operation == raiseType) {
		log("Se recibio el mensaje de incremento de marea.", 3);
		this->raiseTideAndInformCourts(message->getOperationValue());
		shouldEnd = false;
	} else if (operation == lowType) {
		log("CourtManager: Se recibio el mensaje de decremento de marea.", 3);
		this->lowerTideAndInformCourts(message->getOperationValue());
		shouldEnd = false;
	} else if (operation == closeCourts) {
		log("CourtManager: Se recibio el mensaje de cerrado de cancha.", 3);
		shouldEnd = true;
	} else {
		log("CourtManager: Warning ** Se ignora el mensaje.", 2);
		shouldEnd = false;
	}
	return shouldEnd;
};

void CourtManager::raiseTideAndInformCourts(int amountToRaise) {
	int previousTideLevel = this->tideLevel;
	log("CourtManager: Aumentando marea " + std::to_string(previousTideLevel) + " en " + std::to_string(amountToRaise), 3);
	// Actualizo el valor de la marea (o es mayor o es igual al anterior)
	this->tideLevel = this->getUpdatedTideLevel(amountToRaise);
	if (previousTideLevel < this->tideLevel) {
		log("CourtManager: Aumentando la marea.", 2);
		for (int rowNumber = previousTideLevel; rowNumber < this->tideLevel; ++rowNumber) {
			for (int columnNumber = 0; columnNumber < this->columns; ++columnNumber) {
				this->beach[rowNumber][columnNumber]->closeCourt();
			}
		}
	} else {
		log("CourtManager: Warning ** El valor para actualizar la marea es incorrecto. Ignorando.", 2);
	}
};

void CourtManager::lowerTideAndInformCourts(int amountToLower) {
	int previousTideLevel = this->tideLevel;
	log("CourtManager: Disminuyendo marea " + std::to_string(previousTideLevel) + " en " + std::to_string(amountToLower), 3);
	// Actualizo el valor de la marea (o es menor o es igual al anterior)
	this->tideLevel = this->getUpdatedTideLevel((-1) * amountToLower);
	if (previousTideLevel > this->tideLevel) {
		// Voy desde la cancha que estaba indundada hasta la 
		log("CourtManager: Disminuyendo marea.", 3);
		for (int rowNumber = previousTideLevel; rowNumber > this->tideLevel; --rowNumber) {
			for (int columnNumber = 0; columnNumber < this->columns; ++columnNumber) {
				this->beach[rowNumber][columnNumber]->openCourt();
			}
		}
	} else {
		log("CourtManager: Warning ** El valor para disminuir la marea produce un resultado negativo. Ignorando.", 2);
	}
};

int CourtManager::getUpdatedTideLevel(int amountToAdd) {
	int temp = this->tideLevel + amountToAdd;
	if ( temp >= 0 && temp <= this->rows ) {
		return temp;
	} else {
		return this->tideLevel;
	}
};

void CourtManager::shutDownCourts() {
	log("CourtManager: Abriendo fifo para cerrar las canchas", 3);
	FifoEscritura* fifoMatches = new FifoEscritura("/tmp/fifoMatches");
	fifoMatches->abrir();
	log("CourtManager: Fifo abierto", 3);
	struct messageMatch closeMessage = (const struct messageMatch){ CLOSE };
	log("CourtManager: Cerrando " + std::to_string(this->rows * this->columns) + " canchas.", 3);
	for (int rowNumber = 0; rowNumber < this->rows; ++rowNumber) {
		for (int columnNumber = 0; columnNumber < this->columns; ++columnNumber) {
			CourtCM* court = this->beach[rowNumber][columnNumber];
			if (court->isCourtOpen()) {
				log("CourtManager: Cancha abierta, cerrando. Cancha nro.:" + std::to_string(rowNumber) + " " + std::to_string(columnNumber) , 3);
				int bytes = fifoMatches->escribir(static_cast<void*>(&closeMessage), sizeof(closeMessage));
				if (bytes <= 0) {
					log("** Error ** En la escritura. Errno: ", strerror(errno), 1);
				} else {
					log("Escritura correcta. Cantidad de bytes: ", bytes, 3);
				}
				delete(court);
			} else {
				log("CourtManager: Cancha cerrada, ignorando. Cancha nro.:" + std::to_string(rowNumber) + " " + std::to_string(columnNumber) , 3);
			}
		}
	}
	// Aca falta un semaforo que tienen disminuir las canchas cuando se destruyen.
	// Y aumentar cuando se crean.
	// Aca se espera a que sea cero y se puede seguir la ejecucion.
	// Sino se va a cerrar el archivo y bloquear todas las canchas.
	log("CourtManager: Cerrando fifo de canchas", 3);
	fifoMatches->cerrar();
	delete(fifoMatches);
	log("CourtManager: Fifo de canchas cerrado", 3);
};

// Destructor
CourtManager::~CourtManager() {
	log("CourtManager: Liberando recursos.", 3);
	log("CourtManager: Cerrando canchas.", 3);
	this->shutDownCourts();
	log("CourtManager: Canchas cerradas.", 3);
	log("CourtManager: Cerrando fifo.", 3);
	this->fifoCourtManager->cerrar();
	this->fifoCourtManager->eliminar();	
	delete(this->fifoCourtManager);
	log("CourtManager: Fifo cerrado.", 3);
	log("CourtManager: Recursos liberados.", 3);
}
