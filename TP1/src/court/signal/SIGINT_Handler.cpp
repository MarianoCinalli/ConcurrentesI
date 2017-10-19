#include "court/signal/SIGINT_Handler.h"
#include "court/Court.h"

SIGINT_Handler::SIGINT_Handler(Court* court) : gracefulQuit(0) {
	this->court = court;
}

SIGINT_Handler::~SIGINT_Handler() {
}

sig_atomic_t SIGINT_Handler::getGracefulQuit () const {
	return this->gracefulQuit;
}

/*
 *	Si el partido no termino (Se informa que no termino):
 *		Si se leyó un mensaje de partido. Y no se creo el partido o si el partido fue interrumpido.
 *		Se marca el partido como no finalizado (crearlo de ser necesario). Y enviar los mensajes.
 *		
 *	Si el partido termino, (se envian los mensajes con los resultados):
 *  	Continua la ejecucion de codigo, para enviar los mensajes, y termina por graceful quit.
 *
 */
void SIGINT_Handler::handle() {
	log("SIGINT HANDLER: Manejando marea.", 3);
	bool shouldExit = true;
	messageMatch courtMessage = this->court->getReadMessage();
	Match* courtMatch = this->court->getMatch();
	// Si no esta instanciado el mensaje leido del fifo (valor de inicializacion). Salgo.
	if (courtMessage.operation != IGNORE) {
		// Si la operacion no es jugar salgo.
		if (courtMessage.operation == PLAY) {
			// Valido el estado del juego.
			if (courtMatch != NULL) {
				//  El partido empezo.
				log("SIGINT HANDLER: (Marea) Hay mensaje y comenzo el partido.", 3);
				if (courtMatch->isFinished()) {
					// Si termino el partido, dejo que se envien los mensajes.
					// Y salgo del while.
					log("SIGINT HANDLER: (Marea) Hay mensaje, el patido termino. Enviando mensajes de finalizacion.", 3);
					shouldExit = false;
				} else {
					// Si no termino. Lo cancelo e informo.
					log("SIGINT HANDLER: (Marea) El partido no termino. Informando que se cancelo.", 3);
					courtMatch->cancelMatch();
					log("Court: Partido finalizado. Resultado: ", courtMatch, 3);
					this->court->sendMessages(courtMatch);
					delete(courtMatch);
				}
			} else {
				// El partido no empezo.
				log("SIGINT HANDLER: (Marea) Hay mensaje, pero no partido. Informando que se cancelo.", 3);
				// Crep un obj mensaje para crear un partido.
				Message* message = new Message(courtMessage);
				// Creo el partido.
				courtMatch = this->court->getMatchFromMessage(message);
				// Cancelo el partido.
				courtMatch->cancelMatch();
				// Envio los mensajes.
				this->court->sendMessages(courtMatch);
				delete(courtMatch);
				delete(message);
			}
		} else {
			log("SIGINT HANDLER: (Marea) Hay mensaje, pero no era de nuevo juego. Terminando", 3);
		}
	} else {
		log("SIGINT HANDLER: (Marea) No hay mensaje. Terminando.", 3);
	}
	log("SIGINT HANDLER: Marea manejada.", 3);
	if (shouldExit) {
		delete(this->court);
		exit(EXIT_SUCCESS);
	}
};