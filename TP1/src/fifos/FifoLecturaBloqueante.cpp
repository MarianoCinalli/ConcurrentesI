#include "fifos/FifoLecturaBloqueante.h"
#include <errno.h>
#include <iostream>
#include <string.h>


FifoLecturaBloqueante::FifoLecturaBloqueante(const std::string nombre) : FifoLectura(nombre), lock(nombre + "_lock") {
}

FifoLecturaBloqueante::~FifoLecturaBloqueante() {
}

ssize_t FifoLecturaBloqueante::leer(void* buffer,const ssize_t buffsize) {
	int numberOfBytesRead;
	log("FifoLecturaBloqueante: Obteniendo lock.", 3);
	this->lock.tomarLock();
	log("FifoLecturaBloqueante: Leyendo.", 3);
	numberOfBytesRead = read ( fd,buffer,buffsize );
	log("FifoLecturaBloqueante: Liberando lock.", 3);
	this->lock.liberarLock();
	log("FifoLecturaBloqueante: Lock liberado.", 3);
	return numberOfBytesRead;
}
