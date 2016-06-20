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

using namespace std;

int node_id= -1;
char *inFile=NULL,*outFile=NULL;
int HELLO_INTERVAL=1;
int LSA_INTERVAL=0;
int SPF_INTERVAL=0;
int N_nodes=0,N_edges=0;
int PORT_NUM=node_id+20000;
std::vector<pair<int, pair<int,int> > > *vect;
int so_reuseaddr = 1;
int sock_send,sock_rec;
typedef struct Packet{
	char data[10];
	int srcid;
}packet; 
void parse_command(int argc,char**argv){
	int i=0;
    for (i=1;i<argc;i++){
    	switch(argv[i][1]){
    		case 'i':
    		    if(++i <= argc)
    		    	node_id=atoi(argv[i]);
    		    break;

    		case 'f':
    		    if(++i <=argc)
    		    	inFile=argv[i];
    		    break;

    		case 'o':
    		    if(++i <=argc)
    		    	outFile=argv[i];
    		    break;

    		case 'h':
    		    if(++i <= argc)
    		    	HELLO_INTERVAL=atoi(argv[i]);
    		    break;

    		case 'a':
    		    if(++i <= argc)
    		    	LSA_INTERVAL=atoi(argv[i]);
    		    break;

    		case 's':
    		    if (++i <= argc)
    		    	SPF_INTERVAL=atoi(argv[i]);
    		    break;

    		default:
    		    break;
    	}
    }        
}
std::vector<pair<int,pair<int,int > > > *func(char*filename){
    FILE*fp=fopen(inFile,"r");
    int i,j=0;
    char string[15],ch;
    std::vector<pair <int ,pair<int,int> > > *vect;

    ch=fscanf(fp,"%s",string);
    if(ch!=EOF)
    	N_nodes=atoi(string);
    ch=fscanf(fp,"%s",string);
    if(ch!=EOF)
    	N_edges=atoi(string);
    int num1,num2,num3,num4;
    pair<int,int> myPair;
    pair<int,pair<int,int> >pair_;
    vect=new std::vector< pair <int, pair<int,int > > >[N_nodes];
    while((ch=fscanf(fp,"%d%d%d%d",&num1,&num2,&num3,&num4))!=EOF){
    		myPair=make_pair(num3,num4);
    	    pair_=make_pair(num2,myPair);
    	    vect[num1].push_back(pair_);
    	    pair_=make_pair(num1,myPair);
    	    vect[num2].push_back(pair_);
    }
    /*printf("%d\n",N_nodes);
    for(i=0;i< N_nodes;i++){
       printf("%d \n",vect[i].size());
       for(j=0;j<vect[i].size();j++){
       	  myPair=vect[i][j].second;
       	  cout << i <<" " << vect[i][j].first <<" " << myPair.first <<" " <<myPair.second <<endl;
       }
    }*/
    return vect;
}
void* send_packet(void *addr){
	packet pkt;
	struct sockaddr_in *server_addr=(struct sockaddr_in*)addr;
	strcpy(pkt.data,"HELLO");
	pkt.srcid=node_id;
	while(1){
	sleep(1);
     sendto(sock_send,pkt.data, strlen(pkt.data), 0,
                (struct sockaddr*)server_addr, sizeof (struct sockaddr));
     printf("sending packet to :%d, %s\n",ntohs(server_addr->sin_port),inet_ntoa(server_addr->sin_addr));
   }
   return NULL;
}

void connect(){
	struct sockaddr_in server_addr, client_addr;
    
    if ((sock_rec = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT_NUM);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server_addr.sin_zero), 8);

    if (bind(sock_rec, (struct sockaddr *) &server_addr,
            sizeof (struct sockaddr)) == -1) {
        perror("Binding failed!!");
        exit(1);
    }

    // sender 
    if ((sock_send = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket creation failed");
        exit(1);
    }
}
void *sender(void *param){
    struct hostent *host;
    int neighbors_=vect[node_id].size();
    struct sockaddr_in server_addr[neighbors_];
    int i,j,k;
    
    host = (struct hostent *) gethostbyname("localhost");

    for(i=0;i<vect[node_id].size();i++){
    	server_addr[i].sin_family = AF_INET;
        server_addr[i].sin_addr = *((struct in_addr *) host->h_addr);
        bzero(&(server_addr[i].sin_zero), 8);
    	server_addr[i].sin_port = htons(vect[node_id][i].first+20000);
    }
    printf("Router %d ready to send packets\n",node_id);
    pthread_t threads_id[neighbors_];
    int rc[neighbors_];
    sleep(2);
    for(i=0;i<neighbors_;i++){
    	if((rc[i]=pthread_create(&threads_id[i],NULL,&send_packet,(void*)&server_addr[i])))
    		printf("Pakcet sender thread creation failed\n");
    }
    while (1) {
    	sleep(1);
    } 
    return NULL;
}
void *reciever(void *param){
	int bytes_read;
	int data_size=1024;
	char recv_data[data_size];
	socklen_t addr_len = sizeof (struct sockaddr);
	struct sockaddr_in  client_addr;
    
    printf("\nRouter %d waiting for packets at %d\n",node_id,PORT_NUM);
    fflush(stdout);
    while (1) {
    	sleep(1);
		printf("came here\n");
	    bytes_read = recvfrom(sock_rec, recv_data, data_size-1, 0,
	                (struct sockaddr *) &client_addr, &addr_len);
	    printf("here too\n");
	    recv_data[bytes_read] = '\0';

	    printf("\n(%s , %d) said : ", inet_ntoa(client_addr.sin_addr),
	                ntohs(client_addr.sin_port));
	    printf("%s", recv_data);
	        fflush(stdout);
	}
}
int main(int argc, char *argv[]) {
    parse_command(argc,argv);
    vect=func(inFile);
    PORT_NUM=node_id+20000;
    
    pthread_t sender_thread,receiver_thread;
    int sender_rc,reciever_rc;
    if((reciever_rc=pthread_create(&receiver_thread,NULL,&reciever,NULL)))
    	printf("reciever_thread creation failed\n");
    if((sender_rc=pthread_create(&sender_thread,NULL,&sender,NULL)))
    	printf("sender_thread creation failed\n");
    while(1){
        printf("Main\n");
        sleep(1);
    }
    pthread_join(reciever_rc,NULL);
    pthread_join(sender_rc,NULL);
       
    return 0;
}