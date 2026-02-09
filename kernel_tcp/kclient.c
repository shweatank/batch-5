#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/net.h>
#include <linux/in.h>
#include <linux/socket.h>
#include <net/sock.h>

#define PORT 9090
#define BUF_SIZE 128

static struct socket *sock;
static struct task_struct *client_thread;

static int client_fn(void *data)
{
    struct sockaddr_in addr;
    struct msghdr msg = {};
    struct kvec vec;
    char buf[BUF_SIZE];

    sock_create_kern(&init_net, AF_INET, SOCK_STREAM,
                     IPPROTO_TCP, &sock);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = htons(PORT);

    kernel_connect(sock, (struct sockaddr *)&addr,
                   sizeof(addr), 0);

    printk(KERN_INFO "Kernel client connected\n");

    while (!kthread_should_stop()) {
        strcpy(buf, "Hello from kernel client");

        vec.iov_base = buf;
        vec.iov_len  = strlen(buf);

        kernel_sendmsg(sock, &msg, &vec, 1,
                       vec.iov_len);

        memset(buf, 0, BUF_SIZE);
        vec.iov_len = BUF_SIZE;

        kernel_recvmsg(sock, &msg, &vec, 1,
                       BUF_SIZE, 0);

        printk(KERN_INFO "Client received: %s\n", buf);

        msleep(1000);
    }

    return 0;
}

static int __init kclient_init(void)
{
    client_thread = kthread_run(client_fn, NULL, "kclient_thread");
    return PTR_ERR_OR_ZERO(client_thread);
}

static void __exit kclient_exit(void)
{
    if (client_thread)
        kthread_stop(client_thread);

    if (sock)
        sock_release(sock);

    printk(KERN_INFO "Kernel client unloaded\n");
}

module_init(kclient_init);
module_exit(kclient_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechDhaba");
MODULE_DESCRIPTION("Kernel TCP Client");
