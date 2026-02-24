#include <stdio.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

int main(int argc,char **argv) {
	int id;
	id = msgget(1,IPC_CREAT|IPC_EXCL|0666);
	if(id < 0) {
		perror("msgget\n");
		return 0;
	}
	//printf("%d\n",id);
	struct mymsg {
		long msgtype;
		char *msgdata;
	};

	struct mymsg msg;
	msg.msgtype = atoi(argv[1]);
	msg.msgdata = malloc(strlen(argv[2])+1);
	strcpy(msg.msgdata,argv[2]);

}


