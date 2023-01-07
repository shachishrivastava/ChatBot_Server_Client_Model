/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>
#include <bits/stdc++.h>
using namespace std;
//#define atoa(x) #x
int sockfd, newsockfd, portno;

void parse(char *buffer, char **argv1)
{
    while (*buffer != '\0')
    {
        while (*buffer == ' ' || *buffer == '\t' || *buffer == '\n')
        {
            *buffer++ = '\0';
        }
        *argv1++ = buffer;
        while (*buffer != '\0' && *buffer != ' ' && *buffer != '\t' && *buffer != '\n')
            buffer++;
    }
    *argv1 = NULL;
}
void Die(char *mess)
{
    perror(mess);
    exit(1);
}
int execute(char **argv1)
{
    //cout << " start" << getpid();
    pid_t pid;
    int status;
    if ((pid = fork()) < 0)
    { // fork a child process
        printf("*** ERROR: forking child process failed\n");
        exit(1);
    }
    else if (pid == 0)
    {
        cout << getppid();
        arr[j]=getpid();
       
        while (1)
        {
            pid_t pid1;
            int status1;
            if ((pid1 = fork()) < 0)
            { // fork a child process
                printf("*** ERROR: forking child process failed\n");
                exit(1);
            }
            else if (pid1 == 0)
            {
                int newfd = open("serverclient1.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);
                // here the newfd is the file descriptor of stdout (i.e. 1)
                dup2(newfd, 1);
                if (execvp(*argv1, argv1) < 0)
                {
                    printf("***ERROR***\n");
                    exit(1);
                }
                
            }
            else
            {
                wait(&status1) != pid1;
            }
            
        }
        return getpid();
    }
   
}
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    pid_t arr[1000000];
    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *)&serv_addr,
             sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd,
                       (struct sockaddr *)&cli_addr,
                       &clilen);
    if (newsockfd < 0)
        error("ERROR on accept");
    int j = 0;
    while (1)
    {
        char *argv1[64];
        char *store[100];
        bzero(buffer, 255);
        n = read(newsockfd, buffer, 255);
        if (n < 0)
            error("ERROR reading from socket");
        printf("Client: %s\n", buffer);
        parse(buffer, argv1);              //   parse the line
        if (strcmp(argv1[0], "exit") == 0) // is it an "exit"?
            exit(0);

        
        if (strcmp(argv1[0], "kill") == 0)
        {
           
            int x = arr[atoi(argv1[1])];
            char *s;
            if (atoi(argv1[1]) <= j)
            {
                kill(x, SIGKILL);
                //s = "Process Killed Successfully";
            }
          
            continue;
          
        }
        j++;
        if (send(newsockfd, &j, sizeof(j), 0) < 0)
        {
            Die("sorry !");
        }
    
        arr[j] = execute(argv1);
        printf("process id : %d\n", arr[j]);
      
    }
 
    close(newsockfd);
    close(sockfd);
    return 0;
}
