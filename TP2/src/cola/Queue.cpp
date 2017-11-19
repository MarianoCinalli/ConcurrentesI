
#include "cola/Queue.h"
#include <iostream>

Queue::Queue ( const std::string &file, const char letter ){
    this->key = ftok(file.c_str(),letter);
    if(this->key == -1 ){
        perror("Error en ftok");
        log("Error en ftok ", ERROR);   
    }

    this->id  = msgget(this->key, 0777 | IPC_CREAT);
    if(this->id == -1){
        perror("Error al obtener la cola con msgget");
        log("Error al obtener la cola con msgget ", ERROR);
    }
}

Queue::~Queue(){
}


int Queue::write(const void* message,size_t size) const {
    int value = msgsnd(this->id,message,size - sizeof(long),0);
    if( value == -1){
        log("Error al escribir datos en la cola de mensajes --> ",value, ERROR);
    }
    return value;
}

int Queue::read(const long type, void* buffer, size_t size){
    int value = msgrcv(this->id,buffer,size - sizeof(long),type,0);
    if(value == -1 ){
        log("Error al leer datos en la cola de mensajes --> ",value, ERROR);
    }
    return value;
}


int Queue::destroy() const{
    int value = msgctl(this->id,IPC_RMID,NULL);
     if( value == -1 ){
        log("Error al destruir--> ",value, ERROR);
    }
    return value;
}