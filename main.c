#include"common.h"
void main()
{
/*	struct msgbuf_res v;
    int id = msgget(1, IPC_CREAT | 0666);

    printf("Waiting for message...\n");

    if(msgrcv(id, &v, sizeof(v)-sizeof(long), 0, 0) < 0)
    {
        perror("msgrcv");
        return;
    }

    printf("Received: %d\n", v.val);*/


	struct msgbuf_res v;
	int id=msgget(1,IPC_CREAT|0664);
	int i;
		msgrcv(id,&v,sizeof(v)-sizeof(long),0,0);
			switch(v.op)
			{
				printf("Addition=%d\n",v.val); break;
				printf("Subtraction=%d\n",v.val); break;
				printf("Multiplication=%d\n",v.val); break;
				printf("Division=%d\n",v.val); break;
			}
		msgctl(id,IPC_RMID,0);
}

