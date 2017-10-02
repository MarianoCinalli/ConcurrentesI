#include <iostream>
#include <sys/wait.h>


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include <unistd.h>


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>
#include <string.h>




void lockWrite(int fd){
	struct flock structLock;
	structLock.l_type = F_WRLCK;
	structLock.l_whence = SEEK_SET;
	structLock.l_start = 0;
	structLock.l_len = 0;

	int notLock = fcntl(fd,F_SETLKW,&structLock);

/*	if(notLock == -1){
		std::cout<<"no se pudo bloquear "<<std::endl;
	}else{
		std::cout<<"SI pudo bloquear "<<std::endl;
	}
*/
}



void lockRead(int fd){
	struct flock structLock;
	structLock.l_type = F_RDLCK;
	structLock.l_whence = SEEK_SET;
	structLock.l_start = 0;
	structLock.l_len = 0;

	int notLock = fcntl(fd,F_SETLKW,&structLock);
/*
	if(notLock == -1){
		std::cout<<"no se pudo bloquear "<<getpid()<<std::endl;
	}else{
		std::cout<<"SI pudo bloquear "<<getpid()<<std::endl;
	}
*/
}



void unlock(int fd){
	struct flock structLock;
	structLock.l_type = F_UNLCK;
	structLock.l_whence = SEEK_SET;
	structLock.l_start = 0;
	structLock.l_len = 0;

	int notLock = fcntl(fd,F_SETLKW,&structLock);

/*	if(notLock == -1){
		std::cout<<"no se pudo DESbloquear "<<getpid()<<std::endl;
	}else{
		std::cout<<"SI pudo DESbloquear "<<getpid()<<std::endl;
	}
*/
}


void write(int fd, int value){
	lseek(fd, 0,SEEK_SET);
	write(fd, &value,sizeof(int));
}


const int read(int fd){
	int buff;	
	lseek(fd, 0,SEEK_SET);
	read(fd, &buff,sizeof(int));
	return buff;
}


int main(){

    int fd = open("archivoLock", O_RDWR|O_CREAT);

    write(fd,0); //inicialmente cero
    int readValue =-1;

	int iterations = 50000;
	int status;

	/*
	* memoria compartida ver el numero de lectura sobre el archivo  que realiza el padre e hijo

    int *lectura =(int*) mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
       MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	*lectura = 0;	
	*/

	pid_t pid = fork();
	bool child = pid==0;


	if (pid = child){
	
		for (int i = 0;i < iterations;i++){

			lockWrite(fd);
			readValue = (int) read(fd);
			//std::cout<<"valor leido hijo "<<readValue<<" lectura "<<(*lectura)++ <<std::endl;
			write(fd,readValue+1);
			//std::cout<<"valor escrito hijo "<<readValue+1<<std::endl;
			unlock(fd);
		}
	
		std::cout<<"ultimo valor escrito del hijo "<<readValue+1<<std::endl;
		return 0;

	}else{

		for (int i = 0;i < iterations;i++){
			lockWrite(fd);
			readValue = (int)read(fd);
			//std::cout<<"valor leido padre "<<readValue<<" lectura "<<(*lectura)++<<std::endl;
			write(fd,readValue+1);
			//std::cout<<"valor escrito padre "<<readValue+1<<std::endl;
			unlock(fd);
		}

		wait(&status);
	}
	std::cout<<"ultimo valor escrito del padre "<<readValue+1<<std::endl;
	close(fd);	
	return 0;
}



