#include <iostream>
#include <string>
#include "tools/logger.h"
#include "tools/ProcessSpawner.h"

// Services
#include "services/ExchangeRatesService.h"

// Queues
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// Constants ------------------------------------------------------
int LOG_MIN_LEVEL = 1;
std::ofstream LOG_FILE_POINTER;
// Functions ------------------------------------------------------
void executeExchangeRatesService(int parameters[]){
    log("main: Inicio del servicion de cambio de moneda.",INFORMATION);
    ExchangeRatesService* service = new ExchangeRatesService(parameters[0]);
    service->run();
    delete service;
    log("main: Fin del servicio de cambio de moneda.",INFORMATION);
}

typedef void (*functiontype)();

int main(int argc, char* argv[]) {
    // Initialization
    srand(time(NULL)); // Init seed for random
    LOG_FILE_POINTER.open("tp2.log", std::ofstream::app);
    // End Initialization
    logSessionStarted();

    // Creating exchange rates message queue.
    key_t clave = ftok("archiasdvo.txt", 'A');
	int exchangeRatesQueueId = msgget(clave, 0777 | IPC_CREAT | IPC_EXCL);

    ProcessSpawner* processSpawner = new ProcessSpawner();

    // Spawning exchange rates service
    int parameters[1] = {exchangeRatesQueueId};
    pid_t exchangeRatesServicePid = processSpawner->spawnProcess(&executeExchangeRatesService, parameters);

    // EJEMPLO:
    // Le envio un request al servicio.
    messageRequestExchangeRatesService message;
    message.mtype = exchangeRatesServicePid; // El servicio esta identificado por si pid.
    message.replyTo = getpid(); // Y le responde al pid de este proceso.
    strcpy(message.currency, "peso uruguayao");
    message.operationType = SERVICE_OP_ERASE;
	std::cout << "Enviando mensaje al servicio." << std::endl;
	msgsnd(
		exchangeRatesQueueId,
		static_cast<const void*>(&message),
		sizeof(message) - sizeof(long),
		0
	);
	std::cout << "mensaje enviado." << std::endl;
	// Leo la respuesta.
	// El servicio responde al pid de este proceso. 'message.replyTo = getpid()'
	/*
	messageReplyExchangeRatesService reply;
	std::cout << "Esperando por la respuesta." << std::endl;
	msgrcv(
		exchangeRatesQueueId,
		static_cast<void *>(&reply),
		sizeof(reply) - sizeof(long),
		getpid(),
		0
	);

	if (reply.errorId == NOT_FOUND) {
		std::cout << "Error!!!" << std::endl;
	} else {
		std::cout << "Espero un 3 para peso uruguayo, recibi: " << reply.exchangeRate << std::endl;
	}*/

    messageRequestExchangeRatesService endMessage;
    endMessage.mtype = exchangeRatesServicePid; // El servicio esta identificado por si pid.
    endMessage.operationType = SERVICE_OP_END;
	std::cout << "Enviando mensaje de finalizacion al servicio." << std::endl;
	msgsnd(
		exchangeRatesQueueId,
		static_cast<const void*>(&endMessage),
		sizeof(endMessage) - sizeof(long),
		0
	);
	std::cout << "Mensaje enviado." << std::endl;

	/*
		Obs.:
		1 - Tal vez convenga tener una clase (podria ser global) para
		que todos los procesos que le hagan requests al server tengan
		las ids de las colas de mensajes y los PIDS de los servicios.
		2 - El servicio por ahora no muere seguramente le agregue un
		tipo de commando para finalice su ejecucion.
	*/
	std::cout << "Waiting..." << std::endl;
	int status;
	waitpid(exchangeRatesServicePid, &status, WUNTRACED);
	std::cout << "Waited! Status:" << status << std::endl;
	msgctl(exchangeRatesQueueId, IPC_RMID, NULL);
	logSessionFinished();
	return 0;
}
