#include"header.h"
int main()
{
   char s[20];
   mkfifo("data",0664);
   perror("mkfifo");
   printf("Before..\n");
   int id=open("data",O_WRONLY);
   printf("After...\n");
  while(1)
 {
   printf("enter the data\n");
   scanf("%s",s);
   write(id,s,strlen(s)+1);
   if(strcmp(s,"BYE")==0)
   break;
 }
}  
