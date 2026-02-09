#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msg {
    long type;
    int result;
};
int main() {
    int msgid = msgget(1234, 0666 | IPC_CREAT);
    struct msg m;

    msgrcv(msgid, &m, sizeof(m.result), 1, 0);
    printf("Add: %d\n", m.result);

	msgrcv(msgid, &m, sizeof(m.result), 2 ,0);
	printf("Sub:%d\n", m.result);

	msgrcv(msgid, &m, sizeof(m.result), 3, 0);
	printf("Mul:%d\n", m.result);

	msgrcv(msgid, &m, sizeof(m.result), 4,0);
	printf("Div:%d\n", m.result);

	 msgctl(msgid, IPC_RMID, NULL);
    return 0;
}
