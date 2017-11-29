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
    log(SERVER_NAME + " : Inicializador",INFORMATION);
}

Server::~Server(){
    this->mQueue->destroy();
    delete this->mQueue;
}

void Server::execute(){
    while(!this->finalized){
        //escuchar conexiones
        struct messageConection connection;
        log(SERVER_NAME + " :Execute loop",INFORMATION);
        this->mQueue->read(this->mType,static_cast<void*>(&connection),sizeof(messageConection));
        this->createQueryServer(connection.senderType, connection.typeClient);
    }
}


void Server::createQueryServers(){    
}

void Server::assingQueryServer(int clientType){
}


void Server::createQueryServer(int clientType, int typeClient){
    pid_t pid;
    pid = fork();
    bool isChild = pid == 0;
    if(isChild){
        if(typeClient == typesClientConections::CLIENT_CONECTION){
            log(SERVER_NAME + " :Se crea un QUERYSERVER para atender al cliente con id: ",clientType,INFORMATION);
            QueryServer *queryServer = new QueryServer(this->file,this->letter,clientType);
            queryServer->execute();
            log(SERVER_NAME + " :Fin del QUERYSERVER para atender al cliente con id: ",clientType,INFORMATION);
            delete queryServer;
            exit(0);
        }else if(typeClient == typesClientConections::ADMINISTRATOR_CONECTION){
            log(SERVER_NAME + " :Se crea un servidor para atender al Administrador con id: ",clientType,INFORMATION);
            AdministratorServer *administratorServer = new AdministratorServer(this->file,this->letter,clientType);
            administratorServer->execute();
            log(SERVER_NAME + " :Fin del ADMINISTRATORSERVER para atender al cliente con id: ",clientType,INFORMATION);
            delete administratorServer;
            exit(0);
        }
    }else if(pid < 0){
        log(SERVER_NAME + " :Error no se pudo crear un servidor para atender al cliente con id: ",clientType,ERROR);
    }
}

std::string Server::logMemberVariables(){
    std::string registerLog = "mType: " + std::to_string(this->mType) + 
    " Cantidad de servidores fijos: " + std::to_string(this->amountQueryServers) +
    " Cantidad máxima de servidores a crear: " + std::to_string(this->maxQueryServer);
    return registerLog;
}
