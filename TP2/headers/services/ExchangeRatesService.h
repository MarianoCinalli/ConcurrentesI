#ifndef EXCHANGERATESSERVICE_H
#define EXCHANGERATESSERVICE_H

#include <string>

#include "tools/logger.h"
#include "tools/Messages.h"
#include "tools/Constants.h"

// To send message to the queue.
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

class ExchangeRatesService {

private:
	int queueId;

public:
	ExchangeRatesService(int queueId);
	~ExchangeRatesService();
	void run();

private:
	int getExchangeRateForCurrency(int currencyId);
	messageReplyExchangeRatesService getReply(messageRequestExchangeRatesService* messageToReply);
	void waitChilds();
};

#endif // EXCHANGERATESSERVICE_H
