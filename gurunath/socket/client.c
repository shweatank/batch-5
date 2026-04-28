// client_kern.c
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/net.h>
#include <linux/in.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <net/sock.h>
#include <net/net_namespace.h>
#include <linux/inet.h>


#define SERVER_IP "10.198.33.36"
#define PORT 5000
#define BUF_SIZE 1024

static struct socket *client_sock;
static struct task_struct *client_thread;

static int client_fn(void *data)
{
    struct sockaddr_in server_addr;
    struct msghdr msg;
    struct kvec vec;
    char buffer[BUF_SIZE];
    int ret;

    allow_signal(SIGKILL);

    ret = sock_create_kern(&init_net, AF_INET, SOCK_STREAM, IPPROTO_TCP, &client_sock);
    if (ret < 0) {
        printk(KERN_ERR "Socket create failed\n");
        return ret;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = in_aton(SERVER_IP);
    server_addr.sin_port = htons(PORT);

    ret = kernel_connect(client_sock, (struct sockaddr *)&server_addr,
                         sizeof(server_addr), 0);
    if (ret < 0) {
        printk(KERN_ERR "Connect failed\n");
        return ret;
    }

    printk(KERN_INFO "Connected to server\n");

    while (!kthread_should_stop()) {

        snprintf(buffer, BUF_SIZE, "Hello from Kernel Client");
        memset(&msg, 0, sizeof(msg));

        vec.iov_base = buffer;
        vec.iov_len = strlen(buffer);

        kernel_sendmsg(client_sock, &msg, &vec, 1, strlen(buffer));
        printk(KERN_INFO "Message Sent\n");

        memset(buffer, 0, BUF_SIZE);
        vec.iov_len = BUF_SIZE;

        ret = kernel_recvmsg(client_sock, &msg, &vec, 1, BUF_SIZE, 0);
        if (ret > 0)
            printk(KERN_INFO "Reply: %s\n", buffer);

        msleep(2000);
    }

    return 0;
}

static int __init client_init(void)
{
    client_thread = kthread_run(client_fn, NULL, "kern_client");
    printk(KERN_INFO "Kernel Client Module Loaded\n");
    return 0;
}

static void __exit client_exit(void)
{
    if (client_thread)
        kthread_stop(client_thread);

    if (client_sock)
        sock_release(client_sock);

    printk(KERN_INFO "Kernel Client Module Unloaded\n");
}

module_init(client_init);
module_exit(client_exit);
MODULE_LICENSE("GPL");
