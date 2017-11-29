#ifndef MESSAGES_H_
#define MESSAGES_H_

#include "Constants.h"

	// Mensajes para Altas, modificaciones y consultas. ----------------------------
	// Por ahora consideremos que los clientes (el administrador tambien) 
	// conocen las ids de los servicios (por mas que por ahora no la mostremos).
	// operationType = SERVICE_OP_CREATE o SERVICE_OP_UPDATE o SERVICE_OP_READ.
	// En caso de un SERVICE_OP_CREATE se ignora la id (porque se le va a asignar una nueva
	// con los valores de newTemperature, newPressure, newHumidity).
	// En caso de un SERVICE_OP_READ newTemperature, newPressure, newHumidity se ignoran.
	// En caso de un SERVICE_OP_UPDATE se usan id, newTemperature, newPressure, newHumidity.
	// Si una id no esta en la lista del servicio en el mensaje de respuesta se devuelve errorId = NOT_FOUND
	struct messageRequestWeatherService {
		long mtype;
		long replyTo;
		int operationType;
		char city[50];
		int newTemperature;
		int newPressure;
		int newHumidity;
	};

	struct messageRequestExchangeRatesService {
		long mtype;
		long replyTo;
		int operationType;
		char currency[50];
		int newExchangeRate;
	};

	// Mensajes de respuesta -------------------------------------------------------
	// [Temperature] = Celsius - [Pressure] = ATM  - [Humidity] = %
	struct messageReplyWeatherService {
		long mtype;
		int temperature;
		int pressure;
		int humidity;
		int errorId;
	};

	// 1 ARG Peso = exchangeRate currency
	struct messageReplyExchangeRatesService {
		long mtype;
		int exchangeRate;
		int errorId;
	};

	struct messageConection{
		long mtype;
		long senderType;
		enum typesClientConections typeClient;
	};
	
	struct messageQuery{
		long mtype;
		int queryType;
		char query[50];
	};

	struct messageAdministrator{
		long mtype;
		int typeService;	// SERVICE_WEATHER, SERVICE_EXCHANGERATE
		int operationType; 	// SERVICE_OP_ERASE, SERVICE_OP_UPDATE, SERVICE_OP_READ
		char type[50];		// city, currency
		int newTemperature;
		int newPressure;
		int newHumidity;
		int newExchangeRate;
	};

	struct messageReplyOperation{
		long mtype;
		int status;
	};

#endif // MESSAGES_H_
