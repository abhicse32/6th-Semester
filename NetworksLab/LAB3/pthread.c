#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long m_data;

long long convert(char str[]){
    long long k,i,m_data=0;
    int len=strlen(str);
    for(i=0;i<len;i++){
        k=str[i]-48;
        m_data <<= 1;
        m_data |= k;
    }return m_data;
}
//avadhi
char *dec_to_bin(int rem){
    char *str=(char*)malloc(40);
    int i=0,j;
    while(rem){
      j=rem%2;
      rem/=2;
      str[i++]=(char)(j+48);
    }
    str[i]='\0';
    return str;
}
void strrev(char *str_){
    int len=strlen(str_)-1;
    int i=0,j;
    char temp;
    while(i<=len){
         temp=str_[i];
         str_[i]=str_[len];
         str_[len]=temp;
         i++;len--;
    }
}

int algorithm(char message[],int CRC_BIN){
    
    int i,j,k,rem,qotient;
    j=strlen(message);
    rem=0;
    //printf("%d\n",CRC_BIN);
    for(i=0;i<j;i++){
        rem <<=1;
        rem |= message[i]-48;
        char *stre=dec_to_bin(rem);
        if(rem >> 8)
            rem ^=CRC_BIN;
    }
    return rem;   
}

main(int argc, char *argv[]){
    FILE *fp=fopen(argv[1],"r");

    if(fp==NULL){
        fprintf(stderr, "error in opening file\n");
        exit(1);
    }
    
    long long m_data=0;
    char C[]="100000111";
    int CRC_BIN=0,i,j,k;
    char message[45];
    CRC_BIN=convert(C);
    int rem;
    while(fgets(message,34,fp)!=NULL){
        message[32]='\0';
    char message_[45];
    strcpy(message_,message);
    strcat(message,"00000000");
    
    //printf("%s\n",message);
    rem=algorithm(message,CRC_BIN);
    
    char*str_=dec_to_bin(rem);
    int len=strlen(str_);
    if(len<8){
        for(i=len;i<8;i++)
            str_[i]='0';
        }
    strrev(str_);
     strcat(message_,str_);
    //printf("%d\n", rem);
    if(!algorithm(message_,CRC_BIN))
        printf("successful\n");
}
    return 0;
}