#include "services/ExchangeRatesService.h"

ExchangeRatesService::ExchangeRatesService(int queueId) {
	this->queueId = queueId;
	this->table = new ExchangeRatesTable();
};

ExchangeRatesService::~ExchangeRatesService() {
	this->table->saveChanges();
};

int ExchangeRatesService::getExchangeRateForCurrency(int currencyId) {
	return this->table->get(currencyId);
};

messageReplyExchangeRatesService ExchangeRatesService::getReply(messageRequestExchangeRatesService* messageToReply) {
	messageReplyExchangeRatesService reply;
	reply.mtype = messageToReply->replyTo;
	reply.exchangeRate = this->getExchangeRateForCurrency(messageToReply->currencyId);
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

void ExchangeRatesService::run() {
	bool shouldRun = true;
	log("ExchangeRatesService: Runing.", INFORMATION);
	while (shouldRun) {
		// Busco en la cola un mensaje para este servicio (se envian con mtype = pid del servicio)
		log("ExchangeRatesService: Esperando por un nuevo request.", INFORMATION);
		messageRequestExchangeRatesService readMessage = this->getRequest();
		log("ExchangeRatesService: Request recibido.", INFORMATION);
		if (readMessage.operationType == SERVICE_OP_END) {
			log("ExchangeRatesService: Operacion finalizar.", INFORMATION);
			shouldRun = false;
		} else {
			log("ExchangeRatesService: Operacion leer.", INFORMATION);
			this->reply(&readMessage);
		}
	}
};
