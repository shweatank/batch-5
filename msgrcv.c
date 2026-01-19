#include"header.h"
struct msg_buf
{
   long mtype;
   char data[200];
}v; 
int main()
{     
  int id=msgget(4,IPC_CREAT|0664);
  if(id<0)
  {
    perror("msgget");
    return 0;
  }
  printf("id=%d",id);
  v.mtype=1;
  msgrcv(id,&v,sizeof(v.data),1,0);
  printf("%s\n",v.data);          
}
     
