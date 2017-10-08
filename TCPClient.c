// Chris Buckner  men839

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
    struct sockaddr_in host;
    struct sockaddr_in target;

    // create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("---> Could not create a socket\n");

    // zero out structures
    memset(&host, 0, sizeof(struct sockaddr_in));
    memset(&target, 0, sizeof(struct sockaddr_in));

    // host ip and port setup
    host.sin_family = AF_INET;
    host.sin_port = htons(60040);
    host.sin_addr.s_addr = htonl(INADDR_ANY);

    // target ip and port setup
    target.sin_family = AF_INET;
    target.sin_port = htons(PORT);
    target.sin_addr.s_addr = inet_addr("127.0.0.1");

    // socket binding
    if (bind(sockfd, (struct sockaddr *)&host,sizeof(struct sockaddr_in) ) < 0)
        error("---> Problem binding the socket\n");

    // connect to the target using the socket
    if (connect(sockfd, (struct sockaddr *)&target, sizeof(target)) < 0)
        error("---> Problem connecting to target\n");

    printf("Enter message:\n");

    // zero out buffer (no need to manually null terminate string; buffer is all 0's)
    memset(buf, 0, BUF_SIZ);

    // fill buffer
    int bytes_read = read(STDIN_FILENO, buf, BUF_SIZ);
    if (bytes_read < 0)
        error("---> Problem reading from STDIN\n");

    // send the message plus one extra for null termination
    int bytes_sent = send(sockfd, buf, strlen(buf) + 1, 0);
    if (bytes_sent < 0)
        error("---> Problem sending the message\n");

    // read the response and write it to the buffer
    int bytes_recv = recv(sockfd, buf, BUF_SIZ, 0);
    if (bytes_recv < 0)
        error("---> Problem receiving message\n");
    printf("%s\n", buf);
    close(sockfd);
}

// print out errno message
void error(char * mesg) {
    fprintf(stderr, "%s", mesg);
    perror("Error: ");
    exit(-1);
}
