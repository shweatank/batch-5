#include <sys/ipc.h>
#include <sys/msg.h>

struct msg {
    long type;
    int result;
};

int main() {
    int msgid = msgget(1234, 0666 | IPC_CREAT);

    struct msg m;
    m.type = 4;
    m.result = 10 / 5;

    msgsnd(msgid, &m, sizeof(m.result), 0);
    return 0;
}

