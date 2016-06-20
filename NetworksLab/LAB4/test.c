#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define NUMBER 100

int main(){
   srand(time(NULL));
   FILE *fp=fopen("infile","w");
   int str[33];
   int i,j,k;
   for(i=0;i<NUMBER;i++){
   	   for (j=0;j<32;j++)
         str[j]=rand()%2;
       for(k=0;k<32;k++)
       fprintf(fp, "%d",str[k]);
       fprintf(fp, "\n");
   }
   fclose(fp);
   return 0;
}
