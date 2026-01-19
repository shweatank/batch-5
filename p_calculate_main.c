#include <stdio.h>
#include <sys/msg.h>
#include <stdlib.h>

struct msgbuf {
	long mtype;
	int res;
};

int main() {
	struct msgbuf v;
        int id;
        id=msgget(1,IPC_CREAT|0660);
        if(id<0)
        {
                perror("msgget");
                return 0;
        }
	int ret;
	while(ret = msgrcv(id,&v,sizeof(v.res),1,0))
        {
		if(ret == -1) exit(0);
        	printf("Successfully read from MQ\n");
		printf("Output is: %d\n",v.res);
	}
}
