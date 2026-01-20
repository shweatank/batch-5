#include"header.h"
int main()
{
   char s[20];
   mkfifo("data",0664);
   perror("mkfifo");
   printf("Before..\n");
   int id=open("data",O_RDONLY);
   printf("After...\n");
  while(1)
 {

   read(id,s,sizeof(s));
   printf("%s\n",s);
   if(strcmp(s,"BYE")==0)
   break;
 } 
}  
