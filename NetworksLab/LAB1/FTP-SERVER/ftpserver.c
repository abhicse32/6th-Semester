/* tcpserver.c */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#define max_size 1024

char* tilda_to_home(char *path,char recv_data[],int i,int len,int len2){
     char home[]="/home/abhishek";
     int len1=strlen(home);
     //int len2=strlen(path);
     path=(char*) malloc(sizeof(char)*(len2+len1+2));
          strcpy(path,home);
          for(i=i+1;i<len;i++,len1++)
            path[len1]=recv_data[i];
        return path;
}

void command_processing(char recv_data[],char send_data[],int connected){

      memset(send_data,0,sizeof(char)*max_size);
      int len,i=0,j,k,len2,len3;
      
      len=strlen(recv_data);
      for(i=0;i<len && recv_data[i]!=' ';i++)
         send_data[i]=recv_data[i];
      while(i<len && recv_data[i]==' ')i++;
      char *path=recv_data+i;
      char *temp;
      len2=strlen(path);
      if(len2==0)
            path=".";
      else if(path[0]=='~')
         path=tilda_to_home(path,recv_data,i,len,len2);
      // printf("%s\n",path);
     
     i=0;
     DIR *dp=opendir(path);
     struct dirent *sd;
     if(strcmp(send_data,"NLST")==0){
        
        if(dp==NULL){
            strcpy(send_data,"Invalid directory name@#");
            return ;
        }   
        
        while((sd=readdir(dp))!=NULL){
            //printf("%s\n", sd->d_name);
            temp=sd->d_name;
            if(temp[0]=='.')
                continue;
            len=strlen(temp);
            if(i+len < max_size){
            for(j=0;j<len;j++,i++)
                send_data[i]=temp[j];
            send_data[i++]='\n';
        }else{
          
          if(send(connected,send_data,strlen(send_data),0)<0){
             strcpy(send_data,"failed to send the directory contents@#");
             return ;
          }
          //printf("%s\n",send_data);
          memset(send_data,0,sizeof(char)*max_size);
          i=0;
        }
    }
     strcat(send_data,"@#");

} else if(strcmp(send_data,"CWD")==0){
        if(strcmp(path,".")==0)
           path="/";
        if(chdir(path)<0){
            strcpy(send_data,"error: invalid path@#");
            return ;
        }
        strcpy(send_data,path);
        strcat(send_data,"@#");
     } else if(strcmp(send_data,"RETR")==0){
      FILE *fp=fopen(path,"r");
      if(fp==NULL){
         strcpy(send_data,"Error:file could not be found@#");
         return;
      }
      while(fgets(send_data,max_size-1,fp)!=NULL){
           send(connected,send_data,strlen(send_data),0);
       } 
       strcpy(send_data,"@#");
       puts("Data sent successfully"); 
       fclose(fp); 
       } 
     else if(strcmp(send_data,"PWD")==0){
        char *ch;
        if((ch=getcwd(NULL,100))==NULL){
            strcpy(send_data,"unables to find current directory@#");
            return;
        }
        strcpy(send_data,ch);
        strcat(send_data,"@#");
        
        puts("successfully done the PWD");

     }else if(strcmp(send_data,"QUIT")==0){
        strcpy(send_data,"q@#");
        send(connected,send_data,strlen(send_data),0);
           exit(1);
     }else{
       strcpy(send_data,"Invalid command@#");
     }
}
int
main(int argc, char *argv[]) {
    int sock, connected, bytes_received, true = 1;
    char send_data[max_size],send_data2[max_size],
           recv_data2[max_size],recv_data[max_size];
    const char *ch;
        if((ch=getcwd(NULL,100))==NULL){
            puts("undable to find the current directory");
            return 1;
        }
       
       if(chroot(ch)==-1){
          puts("rooting failed");
          exit(1);
       }
       printf("root changed to:%s\n",ch);
    struct sockaddr_in server_addr, client_addr;
    int sin_size;
    if (argc < 2) {
        printf("Enter PortNo\n");
        exit(0);
    }

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed\n");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server_addr.sin_zero), 8);

    if (bind(sock, (struct sockaddr *) &server_addr, sizeof (struct sockaddr)) == -1) {
        perror("Unable to bind\n");
        exit(1);
    }

    if (listen(sock, 5) == -1) {
        perror("Listen failed\n");
        exit(1);
    }

    printf("\nTCPServer Waiting for client on port %s\n", argv[1]);
    fflush(stdout);

      sin_size = sizeof (struct sockaddr_in);
      connected = accept(sock, (struct sockaddr *) &client_addr, &sin_size);
      printf("\ngot a connection  from(%s , %d)\n",
                inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
      char confirmation[]="OK";
    while (1) {
        fflush(stdout);
        if((bytes_received=recv(connected,recv_data,max_size,0))<0){
            puts("Data could not be received");
            exit(1);
        }

        recv_data[bytes_received]='\0';
        if(strlen(recv_data)==0)
          break;
        printf("recieved data: %s\n",recv_data);
        if(strcmp(recv_data,"put")==0){
             printf("Server ready to accept the data.....\n");
             printf("confirmation message sent:\n\n");
             
            if(send(connected,confirmation,strlen(confirmation),0)<0){
               puts("confirmation message sending failed");
               continue;
           }
               while((bytes_received=recv(connected,recv_data,max_size-1,0))>0){
                    recv_data[bytes_received]='\0';
                    bytes_received=strlen(recv_data);
                    if(strcmp(recv_data+bytes_received-2,"@#")==0){
                      recv_data[bytes_received-2]='\0';
                      puts(recv_data);
                      break;  
                    }
                    printf("%s",recv_data);
        }
      }else
        command_processing(recv_data,send_data,connected);
        if(send(connected,send_data,strlen(send_data),0)<0){
        puts("Sending failed");
        exit(1);
    }

}
close(connected);
    close(sock);
    return 0;
}
