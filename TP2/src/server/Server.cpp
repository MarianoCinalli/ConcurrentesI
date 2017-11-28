#include "server/Server.h"
#include "tools/logger.h"

Server::Server(const std::string& file,const char letter,int amountQueryServers,int maxQueryServer){
    this->mQueue = new Queue(file,letter);
    this->mType = 1;
    this->reciverType = -1;
    this->file = file;
    this->letter = letter;
    this->amountQueryServers = amountQueryServers;
    this->maxQueryServer = maxQueryServer;
    this->finalized = false;
}

Server::~Server(){
    this->mQueue->destroy();
    delete this->mQueue;
}

void Server::execute(){
    //this->createQueryServers();

    while(!this->finalized){
        //escuchar conexiones
        struct messageConection conection;
        this->mQueue->read(this->mType,static_cast<void*>(&conection),sizeof(messageConection));
        //if(){
         //   this->assingQueryServer(conection.senderType);
        //}else{
            this->createQueryServer(conection);
        //}
    }
}


void Server::createQueryServers(){    
}

void Server::assingQueryServer(int clientType){
}


void Server::createQueryServer(struct messageConection connection){
    pid_t pid;
    pid = fork();
    bool isChild = pid ==0;
    if(isChild){
        if (connection.typeClient == typesClientConections::CLIENT_CONECTION) {
            log(SERVER_NAME + " : Se crea un servidor para atender al cliente con id: ",connection.senderType,INFORMATION);
            QueryServer *queryServer = new QueryServer(this->file,this->letter,connection.senderType);
            queryServer->execute();
            delete queryServer;
            exit(0);
        } else if (connection.typeClient == typesClientConections::ADMINISTRATOR_CONECTION) {
            log(SERVER_NAME + " : Se crea un servidor para atender al administrador con id: ",connection.senderType,INFORMATION);
            AdministratorServer *administratorServer = new AdministratorServer(this->file,this->letter,connection.senderType);
            administratorServer->execute();
            delete administratorServer;
            exit(0);
        }
    }else if(pid < 0){
        log(SERVER_NAME + " : Error no se pudo crear un servidor para atender al consumidor de servicios con id: ",connection.senderType,ERROR);
    }
}

std::string Server::logMemberVariables(){
    std::string registerLog = "mType: " + std::to_string(this->mType) + 
    " Cantidad de servidores fijos: " + std::to_string(this->amountQueryServers) +
    " Cantidad máxima de servidores a crear: " + std::to_string(this->maxQueryServer);
    return registerLog;
}
