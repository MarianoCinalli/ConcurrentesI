#ifndef WEATHERSERVICE_H
#define WEATHERSERVICE_H

#include <string>

#include "tools/logger.h"
#include "tools/Messages.h"
#include "tools/Constants.h"
#include "services/WeatherTable.h"
#include "services/Weather.h"

// To send message to the queue.
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

class WeatherService {

private:
	int queueId;
	WeatherTable* table;

public:
	WeatherService(int queueId);
	~WeatherService();
	void run();

private:
	Weather* getWeather(std::string city);
	messageReplyWeatherService getReply(messageRequestWeatherService* messageToReply);
	messageRequestWeatherService getRequest();
	void reply(messageRequestWeatherService* messageToReply);
	void erase(messageRequestWeatherService* message);
	void update(messageRequestWeatherService* message);
};

#endif // WEATHERSERVICE_H
