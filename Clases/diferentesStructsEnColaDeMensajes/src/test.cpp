#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <string>
#include <iostream>

struct MensajeNumero {
	long mtype;
	int numero;
};

struct MensajeLetra {
	long mtype;
	char letra;
};

int main(int argc, char* argv[]) {
	const std::string archivo = "test.cpp";
	const char letra = 'a';
	key_t clave = ftok(archivo.c_str(), letra);
	int id = msgget(clave, 0777|IPC_CREAT);
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
	// Lectura
	std::cout << "Leyendo mtype 1, espero un mensaje tipo numero" << std::endl;
	MensajeNumero mensajeNumeroParaLeer;
	msgrcv (id, static_cast<void *>(&mensajeNumeroParaLeer), sizeof(mensajeNumeroParaLeer)-sizeof(long), 1, 0);
	std::cout << "Primer msj leido: " << mensajeNumeroParaLeer.numero << std::endl;

	std::cout << "Leyendo mtype 2, espero un mensaje tipo letra" << std::endl;
	MensajeLetra mensajeLetraParaLeer;
	msgrcv (id, static_cast<void *>(&mensajeLetraParaLeer), sizeof(mensajeLetraParaLeer)-sizeof(long), 2, 0);
	std::cout << "Segundo msj leido: " << (char) mensajeLetraParaLeer.letra << std::endl;

	msgctl(id,IPC_RMID, NULL);
	return 0;
}
