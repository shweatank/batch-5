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

static struct socket *listen_sock;
static struct socket *conn_sock;
static struct task_struct *server_thread;

static int server_fn(void *data)
{
    struct sockaddr_in addr;
    struct msghdr msg = {};
    struct kvec vec;
    char buf[BUF_SIZE];
    int ret;

    ret = sock_create_kern(&init_net, AF_INET, SOCK_STREAM,
                           IPPROTO_TCP, &listen_sock);
    if (ret)
        return ret;

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = htons(PORT);

    kernel_bind(listen_sock, (struct sockaddr *)&addr, sizeof(addr));
    kernel_listen(listen_sock, 1);

    printk(KERN_INFO "Kernel server listening\n");

    kernel_accept(listen_sock, &conn_sock, 0);
    printk(KERN_INFO "Kernel client connected\n");

    while (!kthread_should_stop()) {
        memset(buf, 0, BUF_SIZE);

        vec.iov_base = buf;
        vec.iov_len  = BUF_SIZE;

        ret = kernel_recvmsg(conn_sock, &msg, &vec, 1,
                              BUF_SIZE, 0);
        if (ret <= 0)
            break;

        printk(KERN_INFO "Server received: %s\n", buf);

        strcpy(buf, "ACK from kernel server");
        vec.iov_len = strlen(buf);

        kernel_sendmsg(conn_sock, &msg, &vec, 1,
                       vec.iov_len);
    }

    return 0;
}

static int __init kserver_init(void)
{
    server_thread = kthread_run(server_fn, NULL, "kserver_thread");
    return PTR_ERR_OR_ZERO(server_thread);
}

static void __exit kserver_exit(void)
{
    if (server_thread)
        kthread_stop(server_thread);

    if (conn_sock)
        sock_release(conn_sock);
    if (listen_sock)
        sock_release(listen_sock);

    printk(KERN_INFO "Kernel server unloaded\n");
}

module_init(kserver_init);
module_exit(kserver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechDhaba");
MODULE_DESCRIPTION("Kernel TCP Server");
