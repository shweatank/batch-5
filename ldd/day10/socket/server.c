#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/net.h>
#include <linux/in.h>
#include <linux/slab.h>
#include <linux/socket.h>
#include <linux/uaccess.h>
#include <net/sock.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kernel Socket Server");
MODULE_DESCRIPTION("Kernel TCP Server on PC");

static struct socket *server_sock;
static struct task_struct *server_task;

/* Thread prototype */
static int server_thread(void *data);

static int server_thread(void *data)
{
    struct socket *client_sock = NULL;
    struct sockaddr_in server_addr;
    struct msghdr msg;
    struct kvec iov;
    char *buf;
    int ret;

    /* Allocate buffer */
    buf = kmalloc(128, GFP_KERNEL);
    if (!buf)
        return -ENOMEM;

    /* Create kernel socket */
    ret = sock_create_kern(&init_net, AF_INET, SOCK_STREAM, IPPROTO_TCP, &server_sock);
    if (ret < 0) {
        kfree(buf);
        pr_err("Failed to create server socket\n");
        return ret;
    }

    /* Bind socket */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    ret = kernel_bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        pr_err("Failed to bind server socket\n");
        goto out_release;
    }

    ret = kernel_listen(server_sock, 5);
    if (ret < 0) {
        pr_err("Failed to listen on server socket\n");
        goto out_release;
    }

    pr_info("Kernel server: waiting for client...\n");

    /* Accept a client connection */
    ret = kernel_accept(server_sock, &client_sock, 0);
    if (ret < 0) {
        pr_err("Failed to accept client connection\n");
        goto out_release;
    }

    /* Receive message */
    memset(&msg, 0, sizeof(msg));
    iov.iov_base = buf;
    iov.iov_len = 128;

    ret = kernel_recvmsg(client_sock, &msg, &iov, 1, 128, 0);
    if (ret > 0) {
        buf[ret] = '\0';
        pr_info("Kernel server received: %s\n", buf);
    }

    /* Clean up client */
    if (client_sock)
        sock_release(client_sock);

out_release:
    if (server_sock)
        sock_release(server_sock);
    kfree(buf);
    return ret;
}

static int __init kserver_init(void)
{
    server_task = kthread_run(server_thread, NULL, "kserver_thread");
    return 0;
}

static void __exit kserver_exit(void)
{
    if (server_task)
        kthread_stop(server_task);

    if (server_sock)
        sock_release(server_sock);

    pr_info("Kernel server module unloaded.\n");
}

module_init(kserver_init);
module_exit(kserver_exit);

