// Chris Buckner   men839

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 9000
#define BUF_SIZ 1024

void error(char *);

int main(int argc, char **argv) {
    char buf[BUF_SIZ];
    struct sockaddr_in server;
    struct sockaddr_in client;
    int welcome_sock, new_sock, parent;

    // create socket
    welcome_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (socket < 0)
        error("---> Could not create a socket\n");

    // zero out structures
    memset(&server, 0, sizeof(struct sockaddr_in));
    memset(&client, 0, sizeof(struct sockaddr_in));

    // server ip and port setup
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // socket binding
    if (bind(welcome_sock, (struct sockaddr *)&server,sizeof(struct sockaddr_in) ) == -1)
        error("---> Problem binding the socket\n");

    // listen
    if (listen(welcome_sock, 5) < 0) /* allow 5 requests to queue up */
        error("---> ERROR on listen\n");

    while(1) {
        int clientlen = sizeof(client);

        // accept , create a new socket for the child to handle connection
        new_sock = accept(welcome_sock, (struct sockaddr *)&client, (socklen_t *)&clientlen);
        if (new_sock < 0)
            error("---> Error on accept\n");

        if ((parent = fork()) < 0)
            error("---> Error on fork\n");

        // parent will close its copy of newly created socket
        if (parent) {
            close(new_sock);

        // child will close its version of welcome socket and recv and echo message back
        } else {
            close(welcome_sock);
            char server_message[2048];
            memset(server_message, 0, 2048);
            strcpy(server_message, "Message received at server: ");
            memset(buf, 0, BUF_SIZ);

            int bytes_rec = recv(new_sock, buf, BUF_SIZ, 0);
            if (bytes_rec < 0)
                error("---> Problem receiving message\n");

            strcat(server_message, buf);

            if (send(new_sock, server_message, strlen(server_message) + 1, 0) < 0)
                perror("---> Problem sending message\n");
            break;
        }
    }
}

void error(char * mesg) {
    fprintf(stderr, "%s", mesg);
    perror("Error: ");
    exit(-1);
}
