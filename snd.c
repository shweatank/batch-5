#include"header.h"
int main()
{
  int id=shmget(4,56,IPC_CREAT|0664);
 if(id<0)
 {
   perror("shmget");
   return 0;
 }
 int *p;
  p=shmat(id,0,0);
 if(p<0)
 {
    perror("shmat");
    return 0;
 }
 int i;
 for(i=0;i<5;i++)
 scanf("%d",&p[i]);
 if(shmdt<0)
 {
   perror("shmdt");
   return 0;
 }
 shmdt(p);
}                  
         
