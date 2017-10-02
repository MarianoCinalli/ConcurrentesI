#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

using namespace std;

int main(){
	
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;

	sigaction(SIGINT,&sa,NULL);

	pid_t pid = fork();
	
	bool isChild = pid == 0;

	char* arg[3]={"ls","-l",NULL};

	while(true){
		
		if (isChild){
			cout<<"soy el hijo con pid  "<<getpid()<<endl;
			execvp(arg[0],arg);
		}else{
			cout<<"soy el padre con pid "<<getpid()<<endl;
		}

		sleep(6);
	}

	return 0;
}
