#ifndef FIFOLECTURABLOQUEANTE_H_
#define FIFOLECTURABLOQUEANTE_H_

#include "fifos/Fifo.h"
#include "fifos/FifoLectura.h"
#include "locks/LockFile.h"
#include "tools/logger.h"

class FifoLecturaBloqueante : public FifoLectura {
private:
	LockFile lock;
public:
	FifoLecturaBloqueante(const std::string nombre);
	~FifoLecturaBloqueante();

	ssize_t leer(void* buffer,const ssize_t buffsize);
};

#endif /* FIFOLECTURABLOQUEANTE_H_ */
