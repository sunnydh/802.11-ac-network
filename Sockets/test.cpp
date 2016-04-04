#include <stdio.h>
#include <string.h>

int main ()
{
   FILE *fp2,*fp1;
   char str[1024];
   int read_bytes;

   fp1 = fopen("/home/sunman/Downloads/temp1.txt","r");
   fp2 = fopen( "/home/sunman/Downloads/temp2.txt" , "w+" );

   while((read_bytes = fread(str,1,10,fp1))>0){
   		printf("%d\n",read_bytes);	
   		fwrite(str,read_bytes,1,fp2);
   }

   fclose(fp1);
   fclose(fp2);
  
   return(0);
}