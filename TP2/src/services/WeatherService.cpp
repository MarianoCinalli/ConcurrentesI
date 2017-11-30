#include "services/WeatherService.h"
#include "signals/SIGINT_Handler.h"
#include "signals/SignalHandler.h"

WeatherService::WeatherService(int queueId) {
	this->queueId = queueId;
	this->table = new WeatherTable();
};

WeatherService::~WeatherService() {
	this->table->saveChanges();
};

Weather* WeatherService::getWeather(std::string city) {
	return this->table->get(city);
};

messageReplyWeatherService WeatherService::getReply(messageRequestWeatherService* messageToReply) {
	messageReplyWeatherService reply;
	reply.mtype = messageToReply->replyTo;
	Weather* weather = this->getWeather(messageToReply->city);
	reply.temperature = weather->getTemperature();
	reply.pressure = weather->getPressure();
	reply.humidity = weather->getHumidity();
	if ((reply.temperature < 0) or (reply.pressure < 0) or (reply.humidity < 0)) {
		reply.errorId = NOT_FOUND;
	} else {
		reply.errorId = NO_ERROR;
	}
	return reply;
};

messageRequestWeatherService WeatherService::getRequest() {
	messageRequestWeatherService readMessage;
	msgrcv(
		this->queueId,
		static_cast<void *>(&readMessage),
		sizeof(readMessage) - sizeof(long),
		getpid(),
		0
	);
	return readMessage;
}

void WeatherService::reply(messageRequestWeatherService* messageToReply) {
	messageReplyWeatherService reply = this->getReply(messageToReply);
	msgsnd(
		this->queueId,
		static_cast<const void*>(&reply),
		sizeof(reply) - sizeof(long),
		0
	);
	log("WeatherService: Respuesta envidada.", INFORMATION);
}

void WeatherService::erase(messageRequestWeatherService* message) {
	this->table->erase(
		message->city
	);
}

void WeatherService::update(messageRequestWeatherService* message) {
	this->table->update(
		message->city,
		new Weather(
			message->newTemperature,
			message->newPressure,
			message->newHumidity
		)
	);
}

void WeatherService::run() {
	bool shouldRun = true;
	log("WeatherService: Servicio iniciado.", INFORMATION);
	    // event handler para la senial SIGINT (-2)
    SIGINT_Handler sigint_handler;

    // se registra el event handler declarado antes
    SignalHandler ::getInstance()->registrarHandler(SIGINT, &sigint_handler);
	while (shouldRun && sigint_handler.getGracefulQuit() == 0) {
		// Busco en la cola un mensaje para este servicio (se envian con mtype = pid del servicio)
		log("WeatherService: Esperando por un nuevo request.", INFORMATION);
		messageRequestWeatherService readMessage = this->getRequest();
		log("WeatherService: Request recibido.", INFORMATION);
		switch(readMessage.operationType) {
			case SERVICE_OP_END:
				log("WeatherService: Operacion finalizar.", INFORMATION);
				shouldRun = false;
				break;
			case SERVICE_OP_ERASE:
				log("WeatherService: Operacion eliminar.", INFORMATION);
				this->erase(&readMessage);
				break;
			case SERVICE_OP_UPDATE:
				log("WeatherService: Operacion actualizar.", INFORMATION);
				this->update(&readMessage);
				break;
			case SERVICE_OP_READ:
				log("WeatherService: Operacion leer.", INFORMATION);
				this->reply(&readMessage);
				break;
			default:
				log("WeatherService: Operacion erronea.", INFORMATION);
				break;
		}
	}
	log("WeatherService: Servicio finalizado.", INFORMATION);
};
