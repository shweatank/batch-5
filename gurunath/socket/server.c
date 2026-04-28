// server_kern.c
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/net.h>
#include <linux/in.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <net/net_namespace.h>
#include <net/sock.h>
#include <linux/inet.h>


#define PORT 5000
#define BUF_SIZE 1024

static struct socket *listen_sock;
static struct socket *client_sock;
static struct task_struct *server_thread;

static int server_fn(void *data)
{
    struct sockaddr_in server_addr;
    struct msghdr msg;
    struct kvec vec;
    char buffer[BUF_SIZE];
    int ret;

    allow_signal(SIGKILL);

    ret = sock_create_kern(&init_net, AF_INET, SOCK_STREAM, IPPROTO_TCP, &listen_sock);
    if (ret < 0) {
        printk(KERN_ERR "Socket create failed\n");
        return ret;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    ret = kernel_bind(listen_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        printk(KERN_ERR "Bind failed\n");
        return ret;
    }

    ret = kernel_listen(listen_sock, 5);
    if (ret < 0) {
        printk(KERN_ERR "Listen failed\n");
        return ret;
    }

    printk(KERN_INFO "Kernel Server Listening on port %d...\n", PORT);

    ret = kernel_accept(listen_sock, &client_sock, 0);
    if (ret < 0) {
        printk(KERN_ERR "Accept failed\n");
        return ret;
    }

    printk(KERN_INFO "Client connected!\n");

    while (!kthread_should_stop()) {

        memset(buffer, 0, BUF_SIZE);
        memset(&msg, 0, sizeof(msg));

        vec.iov_base = buffer;
        vec.iov_len = BUF_SIZE;

        ret = kernel_recvmsg(client_sock, &msg, &vec, 1, BUF_SIZE, 0);
        if (ret > 0) {
            printk(KERN_INFO "Received: %s\n", buffer);

            // Echo back
            kernel_sendmsg(client_sock, &msg, &vec, 1, strlen(buffer));
        }

        msleep(1000);
    }

    return 0;
}

static int __init server_init(void)
{
    server_thread = kthread_run(server_fn, NULL, "kern_server");
    printk(KERN_INFO "Kernel Server Module Loaded\n");
    return 0;
}

static void __exit server_exit(void)
{
    if (server_thread)
        kthread_stop(server_thread);

    if (client_sock)
        sock_release(client_sock);

    if (listen_sock)
        sock_release(listen_sock);

    printk(KERN_INFO "Kernel Server Module Unloaded\n");
}

module_init(server_init);
module_exit(server_exit);
MODULE_LICENSE("GPL");
