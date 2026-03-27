#include"header.h"
int main()
{
  int id=shmget(4,1024,IPC_CREAT|0664);
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
  }
   
}     
