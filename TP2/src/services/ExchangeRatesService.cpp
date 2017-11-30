#include "services/ExchangeRatesService.h"
#include "signals/SIGINT_Handler.h"
#include "signals/SignalHandler.h"

ExchangeRatesService::ExchangeRatesService(int queueId) {
	this->queueId = queueId;
	this->table = new ExchangeRatesTable();
};

ExchangeRatesService::~ExchangeRatesService() {
	this->table->saveChanges();
};

int ExchangeRatesService::getExchangeRateForCurrency(std::string currency) {
	return this->table->get(currency);
};

messageReplyExchangeRatesService ExchangeRatesService::getReply(messageRequestExchangeRatesService* messageToReply) {
	messageReplyExchangeRatesService reply;
	reply.mtype = messageToReply->replyTo;
	reply.exchangeRate = this->getExchangeRateForCurrency(messageToReply->currency);
	if (reply.exchangeRate < 0) {
		reply.errorId = NOT_FOUND;
	} else {
		reply.errorId = NO_ERROR;
	}
	return reply;
};

messageRequestExchangeRatesService ExchangeRatesService::getRequest() {
	messageRequestExchangeRatesService readMessage;
	msgrcv(
		this->queueId,
		static_cast<void *>(&readMessage),
		sizeof(readMessage) - sizeof(long),
		getpid(),
		0
	);
	return readMessage;
}

void ExchangeRatesService::reply(messageRequestExchangeRatesService* messageToReply) {
	messageReplyExchangeRatesService reply = this->getReply(messageToReply);
	msgsnd(
		this->queueId,
		static_cast<const void*>(&reply),
		sizeof(reply) - sizeof(long),
		0
	);
	log("ExchangeRatesService: Respuesta envidada.", INFORMATION);
}

void ExchangeRatesService::erase(messageRequestExchangeRatesService* message) {
	this->table->erase(
		message->currency
	);
}

void ExchangeRatesService::update(messageRequestExchangeRatesService* message) {
	this->table->update(
		message->currency,
		message->newExchangeRate
	);
}

void ExchangeRatesService::run() {
	bool shouldRun = true;
	log("ExchangeRatesService: Servicio iniciado.", INFORMATION);
	    // event handler para la senial SIGINT (-2)
    SIGINT_Handler sigint_handler;

    // se registra el event handler declarado antes
    SignalHandler ::getInstance()->registrarHandler(SIGINT, &sigint_handler);
	while (shouldRun && sigint_handler.getGracefulQuit() == 0) {
		// Busco en la cola un mensaje para este servicio (se envian con mtype = pid del servicio)
		log("ExchangeRatesService: Esperando por un nuevo request.", INFORMATION);
		messageRequestExchangeRatesService readMessage = this->getRequest();
		log("ExchangeRatesService: Request recibido.", INFORMATION);
		switch(readMessage.operationType) {
			case SERVICE_OP_END:
				log("ExchangeRatesService: Operacion finalizar.", INFORMATION);
				shouldRun = false;
				break;
			case SERVICE_OP_ERASE:
				log("ExchangeRatesService: Operacion eliminar.", INFORMATION);
				this->erase(&readMessage);
				break;
			case SERVICE_OP_UPDATE:
				log("ExchangeRatesService: Operacion actualizar.", INFORMATION);
				this->update(&readMessage);
				break;
			case SERVICE_OP_READ:
				log("ExchangeRatesService: Operacion leer.", INFORMATION);
				this->reply(&readMessage);
				break;
			default:
				log("ExchangeRatesService: Operacion erronea.", INFORMATION);
				break;
		}
	}
	log("ExchangeRatesService: Servicio finalizado.", INFORMATION);
};
