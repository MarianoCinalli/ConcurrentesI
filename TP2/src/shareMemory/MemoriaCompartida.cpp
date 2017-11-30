#include "shareMemory/MemoriaCompartida.h"
#include "tools/logger.h"
#include "tools/Constants.h"

MemoriaCompartida ::MemoriaCompartida() : shmId(0), ptrDatos(NULL)
{
}

MemoriaCompartida ::~MemoriaCompartida()
{
}

int MemoriaCompartida ::crear(const std::string &archivo, const char letra)
{

	// generacion de la clave
	key_t clave = ftok(archivo.c_str(), letra);
	if (clave == -1)
	{
		log(MEMORIA_COMPARTIDA_NAME + " : Error FTOK", ERROR);
		return ERROR_FTOK;
	}
	else
	{
		// creacion de la memoria compartida
		this->shmId = shmget(clave, sizeof(bool), 0644 | IPC_CREAT);

		if (this->shmId == -1)
		{
			log(MEMORIA_COMPARTIDA_NAME + " : Error SHMGET", ERROR);
			return ERROR_SHMGET;
		}
		else
		{
			// attach del bloque de memoria al espacio de direcciones del proceso
			void *ptrTemporal = shmat(this->shmId, NULL, 0);

			if (ptrTemporal == (void *)-1)
			{
				log(MEMORIA_COMPARTIDA_NAME + " : ERROR SHMAT", ERROR);
				return ERROR_SHMAT;
			}
			else
			{
				this->ptrDatos = static_cast<bool *>(ptrTemporal);
				log(MEMORIA_COMPARTIDA_NAME + " : Memoria Compartida creada exitosamente: ", *(this->ptrDatos), INFORMATION);
				return SHM_OK;
			}
		}
	}
}

void MemoriaCompartida ::liberar()
{
	// detach del bloque de memoria
	shmdt(static_cast<void *>(this->ptrDatos));

	int procAdosados = this->cantidadProcesosAdosados();
	if (procAdosados == 0)
	{
		log(MEMORIA_COMPARTIDA_NAME + " : Se libera Memoria Compartida ", INFORMATION);
		shmctl(this->shmId, IPC_RMID, NULL);
	}
}

void MemoriaCompartida ::escribir(const bool &dato)
{
	log(MEMORIA_COMPARTIDA_NAME + " : Se escribe en Memoria Compartida: ", dato, INFORMATION);
	*(this->ptrDatos) = dato;
}

bool MemoriaCompartida ::leer() const
{
	bool dato = (*(this->ptrDatos));
	log(MEMORIA_COMPARTIDA_NAME + " : Se lee en Memoria Compartida: ", dato, INFORMATION);
	return dato;
}

int MemoriaCompartida ::cantidadProcesosAdosados() const
{
	shmid_ds estado;
	shmctl(this->shmId, IPC_STAT, &estado);
	return estado.shm_nattch;
}