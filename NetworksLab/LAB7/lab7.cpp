#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>
#include <vector>
#include <map>
#include <iostream>
#include <pthread.h>
#include <arpa/inet.h>
#include <math.h>

#define IDLE 1
#define TRANSMIT 2
#define BACKLOG 3

int n_users;
double b_i;
double PACKET_GEN_RATE;
int MAX_PACKETS;


using namespace std;
void parse_command(int argc, char**argv){
    int i=0;
    for (i=1;i<argc;i++){
    	switch(argv[i][1]){
    		case 'N':
    		    if(++i <= argc)
    		    	n_users=atoi(argv[i]);
    		    break;

    		case 'b':
    		    if(++i <=argc)
    		    	b_i=atof(argv[i]);
    		    break;
    		case 'p':
    		    if(++i <=argc)
    		    	PACKET_GEN_RATE=atof(argv[i]);
    		    break;

    		case 'M':
    		    if(++i <= argc)
    		    	MAX_PACKETS=atoi(argv[i]);
    		    break;
    		default:
    		    break;
    	}
    }
}
double min(double a,double b){
	return a>=b ? b: a; 
}
double max(double a,double b){
    return a>=b ? a: b;
}

int main(int argc,char ** argv){

    parse_command(argc,argv);
    //printf("%d %f %d %f\n",n_users,b_i,MAX_PACKETS,PACKET_GEN_RATE);
    int n_pkt=0;
    int arr[n_users];        // each entry in the array represents a user
    double prob_gen,prob_ret;
    int i,j,k;
    // each entry contains the regeneration probability of users
    double prob_b[n_users];
    for(i=0;i<n_users;i++)
    	prob_b[i]=b_i;

    for(i=0; i <n_users;i++)
    	arr[i]=IDLE;
    int trans_count=0;
    long long delay_count=0;
    long long time_slots=0;
    int G;
    srand(time(NULL));
    while(n_pkt<MAX_PACKETS){         
    	//sleep(1);
    	trans_count=0;
    	for(i=0;i<n_users;i++){
    		if(arr[i]== IDLE){
    	       prob_gen=(1.0*rand())/RAND_MAX;
    	       //printf("%f ", prob_gen);
    	       if(prob_gen <= PACKET_GEN_RATE){
    	       	  arr[i]=TRANSMIT;
    	       	  trans_count++;	
    	       }
    		}
    		else if(arr[i]==BACKLOG){
    			prob_ret=(1.0*rand())/RAND_MAX;
    			if(prob_ret <= prob_b[i]){
    				arr[i]=TRANSMIT;
    			    trans_count++;
    			}
    		}
    	}

    	if(trans_count ==1){
    		n_pkt++;
    		for(i=0;i<n_users;i++)
    			if(arr[i]==TRANSMIT){
    				arr[i]=IDLE;
    				prob_b[i]=min(0.75, prob_b[i]*1.15);
    				break;
    			}
    	}else{
    		for(i=0;i<n_users;i++)
    			if(arr[i]==TRANSMIT){
    				arr[i]=BACKLOG;
    				delay_count++;
    				prob_b[i]=max(0.1,prob_b[i]/1.15);
    			}
    	}

       	time_slots++;
    }
    printf("%d %f %f\n",n_users,b_i,PACKET_GEN_RATE);
    printf("average packet delay: %f \n",(1.0*delay_count)/MAX_PACKETS);
    printf("average number of packets sent per slot: %f \n\n\n", MAX_PACKETS/(1.0*time_slots));
    return 0;
}