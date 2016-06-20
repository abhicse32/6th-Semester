#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

long long message_data;
int flag=0;
long long convert(char str[]){
    int k,i;
    long long m_data=0;
    int len=strlen(str);
    for(i=0;i<len;i++){
        m_data <<= 1;
        m_data |= (str[i]-48);
    }return m_data;
}
//avadhi
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

char *dec_to_bin(long long rem){
    char *str=(char*)malloc(34);
    int i=0,j;
    //  printf("%lld\n",rem);
    while(rem){
      j=rem%2;
      rem/=2;
      str[i++]=(char)(j+48);
    }
    str[i]='\0';
    strrev(str);
    return str;
}

long long algorithm(long long CRC_BIN){
    
    long long i,unit=31;
    CRC_BIN <<=23;
    if(flag){
        unit=39; CRC_BIN<<=8;
    }
    for(i=0;i<32;i++){
   //printf("%s\n",dec_to_bin(message_data));
        if((unsigned long long)message_data>>unit)
            message_data^=CRC_BIN;
            message_data<<=1;
    }
    return message_data>>24;   
}

main(int argc, char *argv[]){
    FILE *fp=fopen(argv[1],"r");

    if(fp==NULL){
        fprintf(stderr, "error in opening file\n");
        exit(1);
    }
    FILE* fp2=fopen(argv[2],"w");
    char C[]="100000111";
    long long CRC_BIN=0,i,j,k,rem;
    char message[45];
    CRC_BIN=convert(C);
     //printf("%lld\n",CRC_BIN);
    srand(time(NULL));
    while(fgets(message,34,fp)!=NULL){
        message[32]='\0';
        flag=0;
    fprintf(fp2,"original string: %s\n",message);
    char message_[45];
    long long m_data=convert(message);
    message_data=m_data;
    rem=algorithm(CRC_BIN);
    message_data = m_data<<8;
    message_data |= rem;
    fprintf(fp2,"original string with CRC: %s\n",dec_to_bin(message_data));
    flag=1;
    // generating error
    int error_introduced=0;
    if(rand()%2){
      switch(1+rand()%3){
          case 1:
             message_data ^= (1 << (rand()%38));
             error_introduced=1;
             break;
          case 2:
             message_data ^=(5 << rand()%36);
             error_introduced=2;
             break;
          case 3:
             message_data ^=(11 << rand()%35);
             error_introduced=3;
             break;
          default:
             break;
         }
    }
    fprintf(fp2,"Corrupted String: %s\n",dec_to_bin(message_data));
    fprintf(fp2,"Number of errors introduced : %d\n",error_introduced);
    if(algorithm(CRC_BIN)==0)
    fprintf(fp2,"CRC Check: Passed\n");
    else fprintf(fp2,"CRC Check: Failed\n\n");
}

// taking pain for optional questions 

    return 0;
}
