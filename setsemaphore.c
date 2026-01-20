#include"header.h"
int main(int argc,int**argv)
{
  
  if(argc!=3)
  {
     printf("USAGE: ./set semvalue\n");
     return 0;
  }
   int id=semget(4,5,IPC_CREAT|0644);
    if(id<0)
    {
      perror("semget");
      return 0;
    }
  printf("id=%d\n",id);
  struct sembuf v;
//  v.sem_num=2;
//  v.sem_op=0;
//  v.sem_flg=0;
  int r=semctl(id,atoi(argv[1]),SETVAL,atoi(argv[2]));
  printf("r=%d\n",r);         
}
         
        
