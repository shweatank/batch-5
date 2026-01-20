#include"header.h"
int main()
{   
  struct sembuf v;
  int id=semget(4,5,IPC_CREAT|0664);
 if(id<0)
 {
   perror("semget");
   return 0;
 }
 int p=open("data",O_RDONLY|O_APPEND|O_CREAT|0664);
 if(p<0)
 {
   printf("file not exits\n");
   return 0; 
 }
 v.sem_num=2;
 v.sem_op=0;
 v.sem_flg=0;
 printf("Before\n");
 semop(id,&v,1); 
 semctl(id,2,SETVAL,1);
 printf("After\n");
 char ch;
 for(ch='a';ch<='z';ch++)
 {
   write(p,&ch,1);
   sleep(1);
 }
 semctl(id,2,SETVAL,0);
 printf("done\n");
}              
