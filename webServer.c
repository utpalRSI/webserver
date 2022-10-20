#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>


char webpage[] = "HTTP/1.1 200 OK\nServer: nweb/%d.0\nContent-Length: %ld\nConnection: close\nContent-Type: %s\n\n";


int main(void){
    struct sockaddr_in server_addr, client_addr;
    socklen_t sin_len = sizeof(client_addr);
    int fd_server, fd_client;
    char buf[2048];
    int fding;
    int on = 1;
    
    fd_server = socket(AF_INET, SOCK_STREAM, 0);
    
    if(fd_server < 0){
        perror("Socket fd");
        exit(1);
    }

    setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if(bind(fd_server, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1){
        perror("bind");
        close(fd_server);
        exit(1);
    }

    if(listen(fd_server, 10) == -1){
        perror("listen");
        exit(1);
    }

    while(1){
        fd_client = accept(fd_server, (struct struct_addr *)&client_addr, &sin_len);
        if(fd_client == -1){
            perror("Connection failed....\n");
            continue;
        }
        printf("Got client connection...\n");
    
        if(!fork()){
           /* child process */
           close(fd_server);
           memset(buf, 0, 2048);
           read(fd_client, buf, 2047);
           printf("%s\n", buf);
           write(fd_client, webpage, sizeof(webpage) -1);
           close(fd_client);
           printf("Closing...");
           exit(0);
        }
        /* parent process */
        close(fd_client);
    }
}

























