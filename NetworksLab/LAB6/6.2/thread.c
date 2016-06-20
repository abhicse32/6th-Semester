
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

//pthread_rwlock_t ptr_thr_lock = PTHREAD_RWLOCK_INITIALIZER;

typedef struct {
        int t;
} thread_arguments;

void *thread_stuff(void *args) {
        thread_arguments *t_args;
        int tid;

        t_args = (thread_arguments *)args;
        //pthread_rwlock_rdlock(&ptr_thr_lock);
        tid = t_args->t;
        //pthread_rwlock_unlock(&ptr_thr_lock);

        /*while (1) {
        sleep (1);*/
        printf("Thread #%i!\n", tid);
        /*}*/

        t_args = NULL;
        pthread_exit(NULL);
        return NULL;
}

int wait_threads(pthread_t threads[], thread_arguments *t_args[], int nthreads) {
        int t;
        int rc;

        // Waiting for threads termination
        for(t=0; t<nthreads; t++) {
                rc = pthread_join(threads[t], NULL);
                free(t_args[t]);
                if (rc != 0) {
                        printf("Error waiting for termination of thread %i: %i\n", t, rc);
                        return 1;
                        break;
                }
        }

        return 0;
}

int spawn_threads(pthread_t threads[], thread_arguments *t_args[], int nthreads) {
        int t;
        int rc;

        // Spawning threads
        for(t=0; t<nthreads; t++) {
                t_args[t] = (thread_arguments *) malloc(sizeof(thread_arguments));
                //pthread_rwlock_wrlock(&ptr_thr_lock);
                t_args[t]->t = t;
                //pthread_rwlock_unlock(&ptr_thr_lock);

                printf("Spawning thread: %i\n", t);
                rc = pthread_create(&threads[t], NULL, (void *)thread_stuff, (void *)t_args[t]);
                if (rc != 0) {
                        printf("Error spawning thread %i: %i\n", t, rc);
                        wait_threads(threads, t_args, rc+1);
                        return t+1;
                        break;
                }
        }

        return 0;
}

int main() {
        pthread_t threads[20];
        thread_arguments *t_args[20];
        int rc;

        rc = spawn_threads(threads, t_args, 20);
        if (rc > 0) {
                printf("Failed spawning thread number %i\n", rc-1);
                return 1;
        }

        rc = wait_threads(threads, t_args, 20);

        return 0;
}


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
using namespace std;

int node_id=-1;
char *inFile=NULL,*outFile=NULL;
int HELLO_INTERVAL=1;
int LSA_INTERVAL=0;
int SPF_INTERVAL=0;
int N_nodes=0,N_edges=0;
int PORT_NUM=node_id+20000;
std::vector<pair<int, pair<int,int> > > *vect;

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
    
    int arr[4];
    
    pair<int,int> myPair;
    pair<int,pair<int,int> >pair_;
    vect=new std::vector< pair <int, pair<int,int > > >[N_nodes];
    while((ch=fscanf(fp,"%s",string))!=EOF){
        arr[j++]=atoi(string);
        j%=4;
        if(!j){
                myPair=make_pair(arr[2],arr[3]);
            pair_=make_pair(arr[1],myPair);
            vect[arr[0]].push_back(pair_);
            pair_=make_pair(arr[0],myPair);
            vect[arr[1]].push_back(pair_);
        }
    }
 /*   for(i=0;i< N_edges;i++){
       for(j=0;j<vect[i].size();j++){
          myPair=vect[i][j].second;
          cout << i <<" " << vect[i][j].first <<" " << myPair.first <<" " <<myPair.second <<endl;
       }
    }*/
    return vect;
}

void *sender(void *param){
      int sock;
    struct hostent *host;
    int neighbors_=vect[node_id].size();
    struct sockaddr_in server_addr[neighbors_];
    char send_data[1024];
    int i,j,k;
    host = (struct hostent *) gethostbyname("localhost");

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket creation failed");
        exit(1);
    }

    for(i=0;i<vect[node_id].size();i++){
        server_addr[i].sin_family = AF_INET;
        server_addr[i].sin_addr = *((struct in_addr *) host->h_addr);
        bzero(&(server_addr[i].sin_zero), 8);
        server_addr[i].sin_port = htons(vect[node_id].first+20000);
    }
    printf("Router %d ready to send packets\n",node_id);
    while (1) {
        
        scanf("%s", send_data);

        if ((strcmp(send_data, "q") == 0) || strcmp(send_data, "Q") == 0)
            break;

        else
            sendto(sock, send_data, strlen(send_data), 0,
                (struct sockaddr *) &server_addr, sizeof (struct sockaddr));
    }  
}
void *reciever(void *param){
    int sock;
    int addr_len, bytes_read;
    char recv_data[1024];
    struct sockaddr_in server_addr, client_addr;
  
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT_NUM);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server_addr.sin_zero), 8);

    if (bind(sock, (struct sockaddr *) &server_addr,
            sizeof (struct sockaddr)) == -1) {
        perror("Binding failed");
        exit(1);
    }

    addr_len = sizeof (struct sockaddr);

    printf("\n Router %d waiting for packets at %s\n",node_id,PORT_NUM);
    fflush(stdout);

    while (1) {
        bytes_read = recvfrom(sock, recv_data, 1024, 0,
                (struct sockaddr *) &client_addr, &addr_len);


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
    int PORT_NUM=node_id+20000;
    /*int sock,sock2,i,j,k,l;
    int addr_len, bytes_read;
    char recv_data[1024],send_data[1024];
    int Num_Neighbors=vect[node_id].size();  // Number of neighbors of the current router 

    struct hostent *host;
    struct sockaddr_in router_addr;
    struct sockaddr_in neighbor_addr[Num_Neighbors];
    
    //accepting connection from the neighbors
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        printf("Socket creation failed at router: %d\n",node_id);
        exit(1);
    }

    router_addr.sin_family = AF_INET;
    router_addr.sin_port = htons(PORT_NUM);
    router_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(router_addr.sin_zero), 8);
    
    if (bind(sock, (struct sockaddr *) &router_addr,
            sizeof (struct sockaddr)) == -1) {
        perror("Binding failed");
        exit(1);
    }

    addr_len = sizeof (struct sockaddr);

    printf("\nRouter %d is ready to accept connections on port : %d\n",node_id,PORT_NUM);
    fflush(stdout);

    //sending connection request to the neighboring routers
    host = (struct hostent *) gethostbyname("localhost");

    for(i=0;i<vect[node_id].size();i++){
        neighbor_addr[i].sin_family = AF_INET;
        neighbor_addr[i].sin_addr = *((struct in_addr *) host->h_addr);
        neighbor_addr[i].sin_port = htons(vect[node_id][i].first + 20000);
        bzero(&(neighbor_addr[i].sin_zero), 8);

    }
    pthread_t sender_thread,receiver_thread;
    int sender_rc,reciever_rc;
*/
    sender_rc=pthread_create(&sender_thread,NULL,&sender,NULL);
    reciever_rc=pthread_create(&receiver_thread,NULL,&reciever,NULL);
    while(1){

    }
    /*while (1) {
        bytes_read = recvfrom(sock, recv_data, 1024, 0,
                (struct sockaddr *) &client_addr, &addr_len);


        recv_data[bytes_read] = '\0';

        printf("\n(%s , %d) said : ", inet_ntoa(client_addr.sin_addr),
                ntohs(client_addr.sin_port));
        printf("%s", recv_data);
        fflush(stdout);
    }*/
    
    return 0;
}