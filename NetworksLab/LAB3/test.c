
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
#include <pthread.h>
#include <time.h>
/*
Command Line Arguments
Port No :argv[1]
IP address :argv[2]
 */
int main(int argc, char *argv[]) {
    int i;
    clock_t start=clock();
    for(i=0;i<10;i++)
        printf("elapsed time: %f\n", (double)(clock()-start));

}
/*
char *DEBUG_OPTION="no_debug";
char *RECEIVER_NAME=NULL;
int  RECEIVER_PORT_NO=-1;
int SEQUENCE_NUMBER_FIELD_LENGTH=3;
int MAX_PACKET_LENGTH=10;//1024;
int PACKET_GEN_RATE=5;
int MAX_PACKETS=20;
int WINDOW_SIZE=8;
int BUFFER_SIZE=8200;

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
                RECEIVER_PORT_NO=atoi(argv[i]+2);
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

*/