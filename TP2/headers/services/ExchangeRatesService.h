#ifndef EXCHANGERATESSERVICE_H
#define EXCHANGERATESSERVICE_H

#include <string>

#include "tools/logger.h"
#include "tools/Messages.h"
#include "tools/Constants.h"
#include "services/ExchangeRatesTable.h"

// To send message to the queue.
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

class ExchangeRatesService {

private:
	int queueId;
	ExchangeRatesTable* table;

public:
	ExchangeRatesService(int queueId);
	~ExchangeRatesService();
	void run();

private:
	int getExchangeRateForCurrency(std::string currency);
	messageReplyExchangeRatesService getReply(messageRequestExchangeRatesService* messageToReply);
	messageRequestExchangeRatesService getRequest();
	void reply(messageRequestExchangeRatesService* messageToReply);
	void erase(messageRequestExchangeRatesService* message);
	void update(messageRequestExchangeRatesService* message);
};

#endif // EXCHANGERATESSERVICE_H
