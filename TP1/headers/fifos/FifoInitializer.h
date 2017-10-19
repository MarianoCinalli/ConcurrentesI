#ifndef FIFOINITIALIZER_H_
#define FIFOINITIALIZER_H_

#include "tools/Constants.h"
#include "fifos/FifoLectura.h"
#include <vector>
#include <iterator>

class FifoInitializer {

	private:
		std::vector<FifoLectura*>* fifos;

	public:
		void createFifos();
		void deleteFifos();
};

#endif // FIFOINITIALIZER_H_
