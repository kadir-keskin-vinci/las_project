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

#define SERVER_IP "127.0.0.1"
#define BUFFERSIZE 1000

int initSocketClient(char *serverIP, int serverPort)
{
    int sockfd = ssocket();
    sconnect(serverIP, serverPort, sockfd);

    return sockfd;
}

int main(int argc, char **argv)
{
    int port = atoi(argv[1]);

    int sockfd = initSocketClient(SERVER_IP, port);

    int pid_lec = sfork();

    if (pid_lec == 0)
    {
        
        while (1)
        {
            char affichage[BUFFERSIZE];

            int n = sread(sockfd, affichage, BUFFERSIZE);

            if (n == 0)
            {
                break;
            }

            printf("resultat de la commande:\n");
            swrite(1, affichage, n);
        }

        printf("Fils du contoller est termine\n");
    }
    else
    {
        
        printf("accepter\n");

        while (1)
        {
            char commande[BUFFERSIZE] = {0x0};

            int r = sread(0, commande, BUFFERSIZE);
            // commande[r]='\0';
            // printf("[CTLR] rad cmd '%s'\n", commande);

            if (r == 0)
            {
                shutdown(sockfd, SHUT_WR);
                break;
            }

            //commande[r - 1] = '\0';
            swrite(sockfd, commande, r);
        }
    }

    sclose(sockfd);
}