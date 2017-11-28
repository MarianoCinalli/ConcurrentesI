#include "services/ExchangeRatesService.h"

ExchangeRatesService::ExchangeRatesService(int queueId) {
	this->queueId = queueId;
	this->table = new ExchangeRatesTable();
};

ExchangeRatesService::~ExchangeRatesService() {
};

int ExchangeRatesService::getExchangeRateForCurrency(int currencyId) {
	return this->table->get(currencyId);
};

messageReplyExchangeRatesService ExchangeRatesService::getReply(messageRequestExchangeRatesService messageToReply) {
	messageReplyExchangeRatesService reply;
	reply.mtype = messageToReply.replyTo;
	reply.exchangeRate = this->getExchangeRateForCurrency(messageToReply.currencyId);
	reply.errorId = NO_ERROR;
	return reply;
};

void ExchangeRatesService::run() {
	bool shouldRun = true;
	messageRequestExchangeRatesService readMessage;
    log("ExchangeRatesService: Runing.", INFORMATION);
	while (shouldRun) {
		// Busco en la cola un mensaje para este servicio (se envian con mtype = pid del servicio)
    	log("ExchangeRatesService: Esperando por un nuevo request.", INFORMATION);
		msgrcv(
			this->queueId,
			static_cast<void *>(&readMessage),
			sizeof(readMessage) - sizeof(long),
			getpid(),
			0
		);
    	log("ExchangeRatesService: Request recibido.", INFORMATION);
		messageReplyExchangeRatesService reply = getReply(readMessage);
		msgsnd(
			this->queueId,
			static_cast<const void*>(&reply),
			sizeof(reply) - sizeof(long),
			0
		);
    	log("ExchangeRatesService: Respuesta envidada.", INFORMATION);
    	shouldRun = false;
	}
};
