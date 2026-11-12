#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"
#define SERVER_PORT  "9098"
int main(int argc,char **argv){
    int pid_ctrl,pid_zombie;
    char* port = SERVER_PORT;
    
    if(argc > 1){
        port = argv[1];
    }
    
    pid_zombie = sfork();
     if(pid_zombie == -1){
        perror("erreur");
        exit(10);
    }

    if(pid_zombie == 0){
        
        execl("./zombie","zombie",port,NULL);
       
        perror("execl zombie");
        exit(EXIT_FAILURE);
    }

    sleep(1);
    
    pid_ctrl = sfork();
     if(pid_ctrl == -1){
        perror("erreur");
        exit(10);
    }

     if(pid_ctrl == 0){
        execl("./client","client",port,NULL);
        perror("execl client");
        exit(EXIT_FAILURE);
    }
   

    

    int status = -1;

    swaitpid(pid_ctrl, &status, 0);

    printf("exit status of the client: %d\n", WEXITSTATUS(status));
     printf("port: %s",port);
    
    kill(pid_zombie, SIGTERM);
    int status_zombie = -1;

    swaitpid(pid_zombie, &status_zombie, 0);
    printf("exit status of the zombie: %d\n", WEXITSTATUS(status_zombie));
    

    printf("Labo fin.\n");

    return 0;
}