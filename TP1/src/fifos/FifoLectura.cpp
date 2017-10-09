#include "fifos/FifoLectura.h"
#include <errno.h>
#include <iostream>
#include <string.h>


FifoLectura::FifoLectura(const std::string nombre) : Fifo(nombre) {
}

FifoLectura::~FifoLectura() {
}

void FifoLectura::abrir() {
	fd = open ( nombre.c_str(),O_RDONLY );
	if (fd < 1) {
		std::cout << "Error!: " << strerror(errno) << std::endl;
	}
}

ssize_t FifoLectura::leer(void* buffer,const ssize_t buffsize) const {
	return read ( fd,buffer,buffsize );
}
