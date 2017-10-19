#include "FifoLectura.h"
#include "FifoEscritura.h"
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
	pid_t pid;
	pid = fork();
	if ( pid == 0 ) {
		// Hijo
		std::cout << "Hijo: Abriendo fifo para lectura." << std::endl;
		FifoLectura* fifoLectura = new FifoLectura("/tmp/test");	
		fifoLectura->abrir();
		std::cout << "Hijo: Fifo abierto para lectura." << std::endl;
		int leido = 1;
		while (leido != 0) {
			std::cout << "Hijo: Leyendo..." << std::endl;
			int readBytes = fifoLectura->leer(static_cast<void*>(&leido), sizeof(leido));
			std::cout << "Hijo: Leidos " << readBytes << " bytes. Valor leido: " << leido << std::endl;
			std::cout << "Hijo: Durmiendo..." << std::endl;
			sleep(2);
		}
		std::cout << "Hijo: Cerrando fifo para lectura." << std::endl;
		fifoLectura->cerrar();
		delete(fifoLectura);
		std::cout << "Hijo: Fifo cerrado." << std::endl;
		return 0;
	} else {
		// Padre
		std::cout << "Padre: Abriendo fifo para escritura." << std::endl;
		FifoEscritura* fifoEscritura = new FifoEscritura("/tmp/test");	
		fifoEscritura->abrir();
		std::cout << "Padre: Fifo abierto para escritura." << std::endl;
		int message = 1;
		for (int i = 0; i < 3; ++i) {
			std::cout << "Padre: Escribiendo 1 en fifo." << std::endl;
			fifoEscritura->escribir(static_cast<void*>(&message), sizeof(message));
			std::cout << "Padre: 1 escrito en fifo." << std::endl;
		}
		message = 0;
		std::cout << "Padre: Escribiendo 0 en fifo." << std::endl;
		fifoEscritura->escribir(static_cast<void*>(&message), sizeof(message));
		std::cout << "Padre: 0 escrito en fifo." << std::endl;
		std::cout << "Padre: Cerrando fifo para escritura." << std::endl;
		fifoEscritura->cerrar();
		delete(fifoEscritura);
		std::cout << "Padre: Fifo cerrado." << std::endl;
		std::cout << "Padre: Esperando al hijo." << std::endl;
		int status;
		waitpid(pid, &status, WUNTRACED);
		std::cout << "Padre: El hijo termino. Estado: " << status << std::endl;
		std::cout << "Padre: Abriendo fifo para lectura. " << std::endl;
		std::cout << "Padre: Espero que explote porque no hay nadie escribiendo. " << std::endl;
		FifoLectura* fifoLectura = new FifoLectura("/tmp/test");
		fifoLectura->abrir();
	}
	return 0;
}
