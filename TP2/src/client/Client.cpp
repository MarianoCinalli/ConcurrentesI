#include "client/Client.h"
#include "tools/logger.h"

Client::Client (const std::string& file,const char letter) :
ClientAbstract(file,letter,typesClientConections::CLIENT_CONECTION){
}

Client::~Client(){
}

void Client::execute(){
    
    bool connect = false;
    std::cout<<"PARA INCIAR LA CONEXIÓN INGRESAR LA OPCIÓN: 1"<<std::endl;
    while(!connect){
        char option = '\0';
        std::cin>>option;
        if(option == optionConnect){
            this->connect();
            connect = true;
        }else{
             std::cout<<"OPCION INCORRECTA"<<std::endl;
        }
    }

    std::cout<<"CONEXIÓN INICIADA"<<std::endl;
    

    while(!this->getFinalizeProcess()){

        std::cout<<"-----------------------OPCIONES:--------------------------"<<std::endl;
        std::cout<<"PARA CONSULTAR EL CLIMA INGRESAR LA OPCIÓN: 2"<<std::endl;
        std::cout<<"PARA CONSULTAR EL EL TIPO DE CAMBIO INGRESAR LA OPCIÓN: 3"<<std::endl;
        std::cout<<"PARA TERMINAR LA CONEXIÓN INGRESAR LA OPCIÓN: 0"<<std::endl;
        std::cout<<"----------------------------------------------------------"<<std::endl;

        char option = '\0';
        std::cin>>option;
    
        switch(option){

            case optionQueryWeather:
                this->solveQueryWeaher();
                break;

            case optionQueryExchangeRate:
                this->solveQueryExchangeRate();
                break;

            case optionDisconnect:
                this->disconnect();
                break;

            default :
                 std::cout<<"opcion ingresada: "<<option<<" incorrecta"<<std::endl;
        }
    }
}

