#ifndef MEMORIACOMPARTIDA_H_
#define MEMORIACOMPARTIDA_H_

#define SHM_OK 0
#define ERROR_FTOK -1
#define ERROR_SHMGET -2
#define ERROR_SHMAT -3

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string>
#include <typeinfo>

class MemoriaCompartida
{
	const std::string MEMORIA_COMPARTIDA_NAME = typeid(MemoriaCompartida).name();

  private:
	int shmId;
	bool *ptrDatos;

	int cantidadProcesosAdosados() const;

  public:
	MemoriaCompartida();
	~MemoriaCompartida();
	int crear(const std::string &archivo, const char letra);
	void liberar();
	void escribir(const bool &dato);
	bool leer() const;
};
#endif /* MEMORIACOMPARTIDA_H_ */