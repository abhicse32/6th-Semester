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
#include <queue>
#include <iostream>
#include <pthread.h>
#include <arpa/inet.h>
#define HELLO_M 1
#define HELLOREPLY_M 2
#define LSA_M  3
#define INFINITE 9999999
using namespace std;

int node_id= 0;
char *inFile=NULL,*outFile=NULL;
int HELLO_INTERVAL=1;
int LSA_INTERVAL=5;
int SPF_INTERVAL=20;
int N_nodes=0,N_edges=0;
int neighbors=0, sequence_num=0;
int PORT_NUM=node_id+20000;
std::vector< pair< int, pair < int, pair<int,int > > > > *vect;
pair <int,int>myPair;

int sock_send,sock_rec;
struct sockaddr_in server_addr1;

typedef struct Packet{
    char message[10];
    int srcid;
}packet;

typedef struct HelloReply{
    char message[10];
    int src_id;
    int dest_id;
    int link_value;
}hello_reply;


std::vector<pair< vector<int> ,int> > RoutingTable;
typedef struct LsaMessage{
    char message[5];
    int src_id;
    int sequence_lsa;
    int num_entries;
    std::vector<pair<int,int> > lsa_vector;
}lsa_message;

lsa_message **lsa_database;
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
int min(int a,int b){
    return a>=b ? b:a;
}
int max(int a,int b){
    return a>=b ? a:b;
}

std::vector<pair<int,pair<int,pair<int,int > > > >*func(char*filename){
    FILE*fp=fopen(inFile,"r");
    int i,j=0;
    char string[15],ch;
    std::vector<pair <int , pair <int, pair<int,int> > > > *vect;

    ch=fscanf(fp,"%s",string);
    if(ch!=EOF)
    	N_nodes=atoi(string);
    ch=fscanf(fp,"%s",string);
    if(ch!=EOF)
    	N_edges=atoi(string);
    int num1,num2,num3,num4,min_,max_;

    pair<int,pair <int, pair<int,int> > >pair_;
    vect=new vector< pair <int, pair <int, pair< int,int > > > >[N_nodes];
    while((ch=fscanf(fp,"%d%d%d%d",&num1,&num2,&num3,&num4))!=EOF){
            min_=min(num3,num4);
            max_=max(num4,num3);
    		myPair=make_pair(min_,max_);
    	    pair_=make_pair(num2,make_pair(INFINITE,myPair));
    	    vect[num1].push_back(pair_);
    	    pair_=make_pair(num1,make_pair(INFINITE,myPair));
    	    vect[num2].push_back(pair_);
    }
    return vect;
}

void print_network(){
    for(int i=0;i< N_nodes; i++){
        for(int j=0;j< vect[i].size(); j++){
            myPair=vect[i][j].second.second;
            cout << i << " " << vect[i][j].first <<" " <<
            " " << vect[i][j].second.first << " " << myPair.first << " " <<myPair.second <<endl; 
        }
    }
}

void initialize_database(){
    for(int i=0;i < N_nodes ;i++)
        lsa_database[i]=NULL;
}

void connect(){
    if ((sock_rec = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Socket");
        exit(1);
    }
    struct sockaddr_in server_addr2;

    server_addr2.sin_family = AF_INET;
    server_addr2.sin_port = htons(PORT_NUM);
    server_addr2.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server_addr2.sin_zero), 8);

    if (bind(sock_rec, (struct sockaddr *) &server_addr2,
            sizeof (struct sockaddr)) == -1) {
        perror("Binding failed!!");
        exit(1);
    }

    struct hostent *host;
    host = (struct hostent *) gethostbyname("localhost");

    if ((sock_send = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }
    server_addr1.sin_family = AF_INET;
    server_addr1.sin_addr = *((struct in_addr *) host->h_addr);
    server_addr1.sin_port = htons(PORT_NUM);
    bzero(&(server_addr1.sin_zero), 8);
    
}

void sender_(void *msg, int msg_type){
    packet *pkt;
    hello_reply *h_reply;
    lsa_message *lsa_msg ;
    switch(msg_type){
        case HELLO_M:
              pkt=(packet*)msg;
              sendto(sock_send, (char*)pkt, sizeof(packet), 0,
                (struct sockaddr *) &server_addr1, sizeof (struct sockaddr));
              break;
        case HELLOREPLY_M:
              h_reply=(hello_reply*)msg;
              //printf("trying to send %s from %d to %d \n",h_reply->message,h_reply->src_id,
                //        h_reply->dest_id);
              sendto(sock_send, (char*)h_reply, sizeof(hello_reply), 0,
                (struct sockaddr *) &server_addr1, sizeof (struct sockaddr));
              break;
        case LSA_M:
              lsa_msg=(lsa_message*)msg;
              sendto(sock_send, (char*)lsa_msg, sizeof(lsa_message), 0,
                (struct sockaddr *) &server_addr1, sizeof (struct sockaddr));
              break;
        default:
           break;
    }
}
void *sender_func(void *temp){
    packet* pkt=new packet;
    strcpy(pkt->message,"HELLO");
    pkt->srcid=node_id;

    while (1) {
        sleep(HELLO_INTERVAL);
        for(int i=0;i <neighbors ;i++){
        server_addr1.sin_port=htons(vect[node_id][i].first + 20000);
        printf("sendt: %s from %d to %d\n",pkt->message,pkt->srcid,vect[node_id][i].first );
        sender_(pkt,HELLO_M);
    }
  }
}

pair<int,int> find_max_min_pair(int node){
    for(int i=0;i < neighbors; i++){
        if(vect[node_id][i].first==node)
            return vect[node_id][i].second.second;
    }
}

void print_database(){
    std::vector<pair<int,int> > temp;
    int j;
    for(int i=0;i<N_nodes;i++){
        printf("%d: ",i);
        if(lsa_database[i]==NULL)
            printf("NULL\n");
        else{
            temp=lsa_database[i]->lsa_vector;
         for(j=0,printf("\n"); j<temp.size(); j++)
            printf("%d %d\n",temp[j].first, temp[j].second);
        printf("\n");
    }
    }
}

void add_lsa_to_database(lsa_message *lsa_){

    int source=lsa_->src_id;
    if(lsa_database[source] ==NULL)
        lsa_database[source]=lsa_;
    else if(lsa_database[source]->sequence_lsa < lsa_->sequence_lsa){
        lsa_message *temp=lsa_database[source];
        lsa_database[source]=lsa_;
        delete temp;
    }
    printf("LSA with source id: %d added to the databse\n",lsa_->src_id);
}

void send_hello_reply(packet *pkt){

    hello_reply*hello_r= new hello_reply;
    
    strcpy(hello_r->message,"HELLOREPLY");
    hello_r->src_id=node_id;
    hello_r->dest_id=pkt->srcid;
    myPair=find_max_min_pair(hello_r->dest_id);

    // calculating the link cost to be between the min and max values 
    // per node 
    hello_r->link_value=myPair.first + rand()%(myPair.second- myPair.first);
    server_addr1.sin_port=htons(hello_r->dest_id + 20000);
    sender_(hello_r,HELLOREPLY_M);
    fflush(stdout);
}

void add_link_cost(char recv_data[]){
    hello_reply* h_reply=(hello_reply*)recv_data;
        for(int i=0;i< neighbors; i++)
            if(vect[node_id][i].first == h_reply->src_id){
                vect[node_id][i].second.first=h_reply->link_value;
                return ;
            }
}

void *reciever_thread(void *temp){
    char recv_data[1024];
    struct sockaddr_in client_addr;
    packet *pkt;
    hello_reply *h_reply;
    lsa_message *lsa_msg;
    socklen_t addr_len = sizeof (struct sockaddr);
    int len_packet=sizeof(packet);
    int len_hello_reply=sizeof(hello_reply);
    int len_lsa_message=sizeof(lsa_message);
    int bytes_read,i;
    while (1) {
        bytes_read = recvfrom(sock_rec, recv_data, 1024, 0,
                (struct sockaddr *) &client_addr, &addr_len);

       // printf("bytes read: %d\n",bytes_read);
        if(bytes_read == len_packet){
            pkt=(packet*)recv_data;
            send_hello_reply(pkt);
        }else if(bytes_read == len_hello_reply){
            add_link_cost(recv_data);
        }else if(bytes_read == len_lsa_message)
            add_lsa_to_database((lsa_message*)recv_data);
   }
    
}

void *send_lsa(void *temp){
    lsa_message* lsa_=new lsa_message;
    strcpy(lsa_->message,"LSA");
    lsa_->src_id=node_id;
    lsa_->num_entries=neighbors;
    int i;
    while(1){
        print_database();
        sleep(LSA_INTERVAL);
        lsa_->lsa_vector.clear();
        lsa_->sequence_lsa=sequence_num;
        for( i=0;i< neighbors ;i++){
            lsa_->lsa_vector.push_back(make_pair(vect[node_id][i].first,
                 vect[node_id][i].second.first));
        }
        add_lsa_to_database(lsa_);
        for(int i=0;i<neighbors ;i++){
            if(i==node_id)
                continue;
            for(int j=0;j< N_nodes; j++){
                if(j!=i){
                 server_addr1.sin_port=htons(vect[node_id][i].first + 20000);
                 //printf("LSA message sent to node: %d\n", vect[node_id][i].first);
                 sender_(lsa_database[j],LSA_M);
                }
            }
        }
        sequence_num++;
    }
}

int main(int argc, char *argv[]){
    parse_command(argc,argv);
    vect=func(inFile);
    PORT_NUM=node_id+20000;
    connect();
    neighbors=vect[node_id].size();
    //print_network();
    lsa_database=new lsa_message*[N_nodes];
    initialize_database();
    /*initialize_routingTable();*/
    int rc_sender1,rc_reciever1;
    pthread_t sender_thread,rec_thread;
    int rc_LSA;
    pthread_t lsa_thread;

    int print_lsa_;
    pthread_t print_lsa_t;

    int rc_routing;
    pthread_t routing_thread;
    printf("\nUDPServer Waiting for client on port %d\n", PORT_NUM);
    fflush(stdout);

    if((rc_reciever1=pthread_create(&rec_thread, NULL, reciever_thread, NULL)))
        perror("reciever thread creation faied!");
    if((rc_sender1=pthread_create(&sender_thread,NULL,sender_func,NULL)))
        printf("sender thread creation failed!\n");

    if((rc_LSA=pthread_create(&lsa_thread, NULL, send_lsa, NULL)))
        perror("lsa_thread creation failed!!");

    /*if((rc_routing=pthread_create(&routing_thread, NULL, print_routing_table,NULL)))
        perror("routing table cannot be displayed");*/
    while(1){  
    } 
    return 0;
}
