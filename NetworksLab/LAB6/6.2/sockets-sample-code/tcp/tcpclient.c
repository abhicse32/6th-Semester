/* tcpclient.c */

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

/*
argv[2]: ipaddress of server
argv[1]: port no of server
 */

int
main(int argc, char *argv[]) {

    int sock, bytes_received;
    char send_data[1024], recv_data[1024];
    struct hostent *host;
    struct sockaddr_in server_addr;
    if (argc < 3) {
        printf("Enter PortNo");
        exit(0);
    }

    host = gethostbyname(argv[2]);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr = *((struct in_addr *) host->h_addr);
    bzero(&(server_addr.sin_zero), 8);

    if (connect(sock, (struct sockaddr *) &server_addr,
            sizeof (struct sockaddr)) == -1) {
        perror("Connect");
        exit(1);
    }

    bytes_received = recv(sock, recv_data, 1024, 0);
    recv_data[bytes_received] = '\0';
    printf("\nRecieved data = %s ", recv_data);

    close(sock);
    
    return 0;
}
