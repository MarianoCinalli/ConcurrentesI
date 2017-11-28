#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

struct MensajeNumero {
	long mtype;
	int numero;
};

struct MensajeLetra {
	long mtype;
	char letra;
};

struct MensajeString {
	long mtype;
	char string[100];
};

struct mensaje {
	long mtype;
	char text[50];
};

int main(int argc, char* argv[]) {
	const std::string archivo = "test.cpp";
	const char letra = 'W';
	/*
	key_t clave = ftok(archivo.c_str(), letra);
	int id = msgget(clave, 0777 | IPC_CREAT | IPC_EXCL);
	// Escritura
	std::cout << "Escribiendo primer mensaje, mtype 1 y numero 1" << std::endl;
	MensajeNumero mensajeNumeroParaEnviar;
	mensajeNumeroParaEnviar.numero = 1;
	mensajeNumeroParaEnviar.mtype = 1;
	msgsnd (id, static_cast<const void*>(&mensajeNumeroParaEnviar), sizeof(mensajeNumeroParaEnviar)-sizeof(long), 0);

	std::cout << "Escribiendo segundo mensaje, mtype 2 y letra 'a'" << std::endl;
	MensajeLetra mensajeLetraParaEnviar;
	mensajeLetraParaEnviar.letra = 'a';
	mensajeLetraParaEnviar.mtype = 2;
	msgsnd (id, static_cast<const void*>(&mensajeLetraParaEnviar), sizeof(mensajeLetraParaEnviar)-sizeof(long), 0);

	std::cout << "Escribiendo tercer mensaje, mtype 3 y string 'Hola mundo!'" << std::endl;
	MensajeString mensajeStringParaEnviar;
	strcpy(mensajeStringParaEnviar.string, "Hola mundo!");
	//mensajeStringParaEnviar.string = "Hola mundo!";
	mensajeStringParaEnviar.mtype = 3;
	msgsnd (id, static_cast<const void*>(&mensajeStringParaEnviar), sizeof(mensajeStringParaEnviar)-sizeof(long), 0);

	std::cout << "Escribiendo cuarto mensaje, mtype 4 y string 'String largo con cosas #!$#%!#$&124as1253t13 asd\"aa!'" << std::endl;
	MensajeString mensajeStringParaEnviar2;
	strcpy(mensajeStringParaEnviar2.string, "String largo con cosas #!$#%!#$&124as1253t13 asd\"aa!");
	//mensajeStringParaEnviar2.string = "String largo con cosas #!$#%!#$&124as1253t13 asd\"aa!";
	mensajeStringParaEnviar2.mtype = 4;
	msgsnd (id, static_cast<const void*>(&mensajeStringParaEnviar2), sizeof(mensajeStringParaEnviar2)-sizeof(long), 0);

	// Lectura
	std::cout << "Leyendo mtype 2, espero un mensaje tipo letra." << std::endl;
	MensajeLetra mensajeLetraParaLeer;
	msgrcv (id, static_cast<void *>(&mensajeLetraParaLeer), sizeof(mensajeLetraParaLeer)-sizeof(long), 2, 0);
	std::cout << "Msj leido: " << (char) mensajeLetraParaLeer.letra << std::endl;

	std::cout << "Leyendo mtype 3, espero un mensaje tipo string." << std::endl;
	MensajeString mensajeStringParaLeer;
	msgrcv (id, static_cast<void *>(&mensajeStringParaLeer), sizeof(mensajeStringParaLeer)-sizeof(long), 3, 0);
	std::cout << "Msj leido: " << mensajeStringParaLeer.string << std::endl;

	std::cout << "Leyendo mtype 1, espero un mensaje tipo numero." << std::endl;
	MensajeNumero mensajeNumeroParaLeer;
	msgrcv (id, static_cast<void *>(&mensajeNumeroParaLeer), sizeof(mensajeNumeroParaLeer)-sizeof(long), 1, 0);
	std::cout << "Msj leido: " << mensajeNumeroParaLeer.numero << std::endl;

	std::cout << "Leyendo mtype 4, espero un mensaje tipo string." << std::endl;
	MensajeString mensajeStringParaLeer2;
	msgrcv (id, static_cast<void *>(&mensajeStringParaLeer2), sizeof(mensajeStringParaLeer2)-sizeof(long), 4, 0);
	std::cout << "Msj leido: " << mensajeStringParaLeer2.string << std::endl;

	std::cout << "eliminando" << std::endl;
	msgctl(id, IPC_RMID, NULL);
	std::cout << "eliminado" << std::endl;
	return 0;
	*/
    key_t clave = ftok(archivo.c_str(), letra);
	int exchangeRatesQueueId = msgget(clave, 0777 | IPC_CREAT | IPC_EXCL);

	std::cout << "Parent: Escribiendo mensaje, mtype 3." << std::endl;
	mensaje mensajeUno;
	mensajeUno.mtype = 3;
	strcpy(mensajeUno.text, "Hola mundo!");
	msgsnd (exchangeRatesQueueId, static_cast<const void*>(&mensajeUno), sizeof(mensajeUno)-sizeof(long), 0);
	int pid = fork();

	if (pid == 0) {
		// Child
		std::cout << "Child: Leyendo mtype 3." << std::endl;
		mensaje mensajeLeerUno;
		msgrcv (exchangeRatesQueueId, static_cast<void *>(&mensajeLeerUno), sizeof(mensajeLeerUno)-sizeof(long), 3, 0);
		std::cout << "Child: Msj leido: " << mensajeLeerUno.text << std::endl;

		mensaje mensajeDos;
		mensajeDos.mtype = 2;
		strcpy(mensajeDos.text, "Good bye word!");
		msgsnd (exchangeRatesQueueId, static_cast<const void*>(&mensajeDos), sizeof(mensajeDos)-sizeof(long), 0);

		std::cout << "Child: Leyendo mtype 3." << std::endl;
		mensaje mensajeLeerDos;
		msgrcv (exchangeRatesQueueId, static_cast<void *>(&mensajeLeerDos), sizeof(mensajeLeerDos)-sizeof(long), 3, 0);
		std::cout << "Child: Msj leido: " << mensajeLeerDos.text << std::endl;

		std::cout << "Child: Fin! " << std::endl;

		exit(0);
	}

	std::cout << "Parent: Leyendo mtype 2." << std::endl;
	mensaje mensajeLeerTres;
	msgrcv (exchangeRatesQueueId, static_cast<void *>(&mensajeLeerTres), sizeof(mensajeLeerTres)-sizeof(long), 2, 0);
	std::cout << "Parent: Msj leido: " << mensajeLeerTres.text << std::endl;

	mensaje mensajeCuatro;
	mensajeCuatro.mtype = 3;
	strcpy(mensajeCuatro.text, "Good bye!");
	msgsnd (exchangeRatesQueueId, static_cast<const void*>(&mensajeCuatro), sizeof(mensajeCuatro)-sizeof(long), 0);

	std::cout << "Parent: Eliminando." << std::endl;
	msgctl(exchangeRatesQueueId, IPC_RMID, NULL);
	std::cout << "Parent: Fin! " << std::endl;
	return 0;
}
