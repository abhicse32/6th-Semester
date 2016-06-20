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
#define max_size 1024
/*
argv[2]: ipaddress of server
argv[1]: port no of server
 */
FILE* fp;
char* tilda_to_home(char *path, int len){
     char home[]="/home/abhishek";
     int len1=strlen(home);
     int i;
     char *path_=(char*) malloc(sizeof(char)*(len+len1+5));
          strcpy(path_,home);
          //puts(path_);
          for(i=1;i<len;i++,len1++)
            path_[len1]=path[i];
          path_[len1]='\0';
        return path_;
}

void change_directory(int i,char str[],int len,char command[]){
   while(i<len && str[i]==' ' || str[i]=='\n')i++;
   char *token = str+i;
   int len2=strlen(token);
   if(len2==0)
     token=".";
   else if(token[0]=='~')
      token=tilda_to_home(token,len-i+1);
   if(strcmp(command,"put")==0){
      fp=fopen(token,"r");
      if(fp==NULL)
        puts("Error:unknown file");
      return;
   }
    if(chdir(token) <0){
         printf("chdir failed: %d\n",errno);
         return ;
    }
    printf("current working directory changed to: %s\n",token); 
    return ;
}

int command_processing(char str[],char str2[]){
      int i,j,k,l=1;
      int len=strlen(str2);
      for(i=0;i<len && str2[i]!=' ';i++)
            str[i]=str2[i];
      if(strcmp(str,"lcd")==0 || strcmp(str,"put")==0){
        change_directory(i,str2,len,str);
        return 0;
      }
      else if(strcmp(str,"ls")==0)
          strcpy(str,"NLST");
       else if(strcmp(str,"cd")==0)
          strcpy(str,"CWD");
      else if(strcmp(str,"get")==0)
            strcpy(str,"RETR");
      
      else if(strcmp(str,"pwd")==0)
          strcpy(str,"PWD");
      else if(strcmp(str,"quit")==0)
          strcpy(str,"QUIT");
      else return ;
      k=strlen(str);
      for(;i<len;i++,k++)
        str[k]=str2[i];
      str[k]='\0';
      return 1;
}

int
main(int argc, char *argv[]) {

    int sock, bytes_received;
    char send_data[max_size], send_data2[max_size],recv_data2[max_size],recv_data[max_size];
    struct hostent *host;
    struct sockaddr_in server_addr;
    if (argc < 3) {
        printf("Enter PortNo\n");
        exit(0);
    }

    host = gethostbyname(argv[2]);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed\n");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr = *((struct in_addr *) host->h_addr);
    bzero(&(server_addr.sin_zero), 8);

    if (connect(sock, (struct sockaddr *) &server_addr,
            sizeof (struct sockaddr)) == -1) {
        perror("Connection failed\n");
        exit(1);
    }
    memset(send_data,0,sizeof(char)*max_size);
    memset(recv_data,0,sizeof(char)*max_size);
    while(1){
    printf("(ftp) ");
    fflush(stdout);
    scanf("%[^\n]s",send_data);
    getchar();
    memset(send_data2,0,sizeof(char)*max_size);
    fp=NULL;
    if(command_processing(send_data2,send_data)==0 && fp==NULL){
       continue;
     }
    if(fp!=NULL){
      if(send(sock,"put",max_size-1,0)<0){
         puts("Can't send the command");
         continue;
      }
      bytes_received=recv(sock, recv_data, max_size-1, 0);
      recv_data[bytes_received]='\0';
      printf("%s\n",recv_data);
      if(strcmp(recv_data,"OK")!=0){
         puts("Server not ready");
         continue;
      }
       while(fgets(send_data2,max_size-1,fp)!=NULL){
        send(sock,send_data2,max_size-1,0);
      }
      strcpy(send_data2,"@#");
      send(sock,send_data2,max_size-1,0);
        fclose(fp);
        continue;
    }
    if(send(sock,send_data2,max_size-1,0)<0){
        puts("Sending failed");
        continue;
    }
    while((bytes_received=recv(sock, recv_data, max_size-1, 0))>0){
      recv_data[bytes_received]='\0';
      if(strcmp(recv_data+bytes_received-2,"@#")==0){
          recv_data[bytes_received-2]='\0';
          if(strcmp(recv_data,"q")==0)
            exit(1);
          puts(recv_data);
         break;
     }else
       puts(recv_data);
   }
  }
    close(sock);
    
    return 0;
}
