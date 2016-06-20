/* udpserver.c */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
/*
Port No has to be given as command line argument 
 */
char *DEBUG_OPTION=NULL;
char*  RECEIVER_PORT_NO=NULL;
int SEQUENCE_NUMBER_FIELD_LENGTH=3;
int PACKET_GEN_RATE=20;
int MAX_PACKETS=30;
int WINDOW_SIZE=8;
int BUFFER_SIZE=10;
float PACKET_ERROR_RATE=0.2;
int error_rate,error_generation_counter=0;

int sock,last_printed=0;
int addr_len, bytes_read;
struct sockaddr_in server_addr, client_addr;
int expecting_packet_num=0,last_sent;
int received_sequence_num=0,buffer_counter=0;
char received_packet[1080];
char ack_message[10];


void parse_command(int argc,char *argv[]){
     int i;
     for(i=1;i<argc;i++){
         switch(argv[i][1]){
            case 'd':
                DEBUG_OPTION="-d";
                break;
            case 'p':
                RECEIVER_PORT_NO=argv[i]+2;
                break;
            case 'n':
                SEQUENCE_NUMBER_FIELD_LENGTH=atoi(argv[i]+2);
                break;
            case 'R':
                PACKET_GEN_RATE=atoi(argv[i]+2);
                break;
            case 'N':
                MAX_PACKETS=atoi(argv[i]+2);
                break;
            case 'W':
                WINDOW_SIZE=atoi(argv[i]+2);
                break;
            case 'B':
                BUFFER_SIZE=atoi(argv[i]+2);
                break;
            case 'e':
                PACKET_ERROR_RATE=atof(argv[i]+2);
                break;
            default:
                break;
         }
     }
}

int main(int argc, char *argv[]) {
    
    if (argc < 2) {
        printf("PortNo Missing");
        exit(0);
    }

    parse_command(argc,argv);
    error_rate=(int)(1.0/PACKET_ERROR_RATE);

   // printf("%d\n",error_rate);
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Socket");
        exit(1);
    }
    
    char **buff=(char**)malloc(sizeof(char*)*(BUFFER_SIZE+1));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(RECEIVER_PORT_NO));
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server_addr.sin_zero), 8);

    if (bind(sock, (struct sockaddr *) &server_addr,
            sizeof (struct sockaddr)) == -1) {
        perror("Bind");
        exit(1);
    }

    addr_len = sizeof (struct sockaddr);

    printf("\nUDPServer Waiting for client on port (%s)\n", RECEIVER_PORT_NO);
    fflush(stdout);
    srand(time(NULL));
    while (1) {
        bytes_read=recvfrom(sock, received_packet, 1079, 0,
            (struct sockaddr *) &client_addr, &addr_len);
        received_packet[bytes_read]='\0';
        if(received_packet[0]=='0')
            received_sequence_num=1;
        else 
            received_sequence_num=received_packet[0]+1;

        ack_message[0]=received_sequence_num;
        printf("\n(%s , %d) is expecting %d\n", inet_ntoa(client_addr.sin_addr),
                              ntohs(client_addr.sin_port),expecting_packet_num);

        if(error_generation_counter >= error_rate){
        if(rand()%2)
           printf("Received: packet error!!\n");
        else 
           printf("Received: \n");
        ack_message[1]='0';
        error_generation_counter=0;
        }else{
           buffer_counter%=BUFFER_SIZE;

          if(buff[buffer_counter]==NULL)
                buff[buffer_counter++]= received_packet+1;
          ack_message[1]=1;
       }
        ack_message[2]='\0';
        printf("Recived : %d %s\n",expecting_packet_num,received_packet+1);
        
        sendto(sock,ack_message, strlen(ack_message), 0,
          (struct sockaddr *) &client_addr, sizeof (struct sockaddr));
        fflush(stdout);
    }
    
    return 0;
}