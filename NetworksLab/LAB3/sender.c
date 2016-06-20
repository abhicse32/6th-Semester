/* udpclient.c */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <math.h>

char *DEBUG_OPTION=NULL;
char *RECEIVER_NAME=NULL;
char *RECEIVER_PORT_NO=NULL;
int SEQUENCE_NUMBER_FIELD_LENGTH=3;
int MAX_PACKET_LENGTH=256;
int PACKET_GEN_RATE=1;
int MAX_PACKETS=10;
int WINDOW_SIZE=7;
int BUFFER_SIZE=10;
int timeout=200000;

int i,j,k,last_ack=0;
int max_retransmission_attempts=0;
float avg_RTT=0;

int buffer_counter=0;
struct sockaddr_in server_addr;
struct hostent *host;
int sock;
clock_t send_time, ack_time;
int sequence_number=8; 
int ack_number=0,last_sent=0,moving_window=0;
char received_ack[10];
int bytes_received;
int addr_len=sizeof (struct sockaddr);
int packet_gen_rate;
short ack_flag=0;
int number_of_acks=0;

void parse_command(int argc,char *argv[]){
     int i;
     for(i=1;i<argc;i++){
         switch(argv[i][1]){
            case 'd':
                DEBUG_OPTION="-d";
                break;
            case 's':
                RECEIVER_NAME=argv[i]+2;
                break;
            case 'p':
                RECEIVER_PORT_NO=argv[i]+2;
                break;
            case 'n':
                SEQUENCE_NUMBER_FIELD_LENGTH=atoi(argv[i]+2);
                break;
            case 'L':
                MAX_PACKET_LENGTH=atoi(argv[i]+2);
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
            default:
               break;
         }
     }
}

void* generate_packet(void* packet_){
    srand(time(NULL));

    int i,j,k;
    char** packet=(char**)packet_;
    int max_len;
    //printf("%d\n",packet_gen_rate);
    for(i=0;i<MAX_PACKETS;i++){
            max_len=40+rand()%MAX_PACKET_LENGTH;
            char* data_=(char*)malloc(sizeof(char)*max_len+2);
            
            buffer_counter=buffer_counter%BUFFER_SIZE;
            if(buffer_counter==0)
            data_[0]='0';
            else 
                data_[0]=buffer_counter;
            
            for(j=1;j<max_len;j++)
                data_[j]=(char)(32+rand()%95);
            //printf("%s %d\n\n",data_,buffer_counter);
            if(packet[buffer_counter]==NULL){
                 packet[buffer_counter]=data_;
                 buffer_counter++;
            }
           //printf("came here a lot\n");
           usleep(packet_gen_rate); 
  }
  pthread_exit(0);
}
void* sender_thread_function(void *packet_){
    usleep(10000);
    char **packet=(char**)packet_;
    clock_t start,end;
    int i=0,j,k;
    while(i<MAX_PACKETS ){
        ack_flag=0;
    if(last_sent < moving_window){
    start=clock();
    printf("%d   %s\n",last_sent,packet[last_sent]);
    sendto(sock, packet[last_sent], strlen(packet[last_sent]), 0,
          (struct sockaddr *) &server_addr, sizeof (struct sockaddr));
    while((unsigned int)((end=clock())-start) < timeout && ack_flag==0);
    if(ack_flag){
         i++; last_sent++;
         moving_window++;
      } 
      last_sent%=BUFFER_SIZE;  
      usleep(packet_gen_rate);
    }else last_sent%=BUFFER_SIZE;
  }
  pthread_exit(0);
}
void* receiver_thread_function(void* packet_){
    int i=0,j,k;
    char ** packet=(char**)packet_;
    while(i<MAX_PACKETS){
    bytes_received = recvfrom(sock, received_ack, 10, 0,
                (struct sockaddr *) &server_addr, &addr_len);
    number_of_acks++;
    received_ack[bytes_received]='\0';
    k=received_ack[0]%BUFFER_SIZE;
    printf("received ack: %d\n",k);
    ack_flag=1;
    if(k==0) k=BUFFER_SIZE;

    k--;
    packet[k]=NULL;
    fflush(stdout);
    i++;
  }
  pthread_exit(0);
}
int main(int argc, char *argv[]) {
    
    if (argc < 3) {
        printf("Enter PortNo");
        exit(0);
    }
    parse_command(argc,argv);
    sequence_number=(int)pow(2,SEQUENCE_NUMBER_FIELD_LENGTH);
    moving_window=WINDOW_SIZE;
    packet_gen_rate=(int)(1000000.0/PACKET_GEN_RATE)+1;
    
    if(WINDOW_SIZE > BUFFER_SIZE){
        printf("Invalid sizes\n");
        exit(1);
    }

    if(argc < 10)
        printf("proceeding with defaut arguments\n");

    char **buff=(char**)malloc(sizeof(char*)*(BUFFER_SIZE+1));

    host = (struct hostent *) gethostbyname((char *) RECEIVER_NAME);
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(RECEIVER_PORT_NO));
    server_addr.sin_addr = *((struct in_addr *) host->h_addr);
    bzero(&(server_addr.sin_zero), 8);
    i=0;
    int rc1,rc2,rc3;

    pthread_t creation_thread,sender_thread,receive_thread;
    if((rc1=pthread_create(&creation_thread,NULL,&generate_packet,buff)))
        printf("packet_generating thread creation failed: %d\n",rc1);

    if((rc2=pthread_create(&sender_thread,NULL,&sender_thread_function,buff)))
        printf("sender thread creation failed: %d\n",rc2);

    if((rc3=pthread_create(&receive_thread,NULL,&receiver_thread_function,buff)))
        printf("receiver thread creation failed: %d\n",rc3);

    while (1) {
        if(max_retransmission_attempts >= 10)
            break;        
        printf("I'm going on\n");
        }
        
        printf("Packet generation rate: %d\n",PACKET_GEN_RATE);
        //printf("Packet length: %d\n",max_len);
        printf("ReTransmision ratio: %f\n",(i+1.0)/12);
        printf("avg_RTT: %f\n",avg_RTT);
    
    return ;
}