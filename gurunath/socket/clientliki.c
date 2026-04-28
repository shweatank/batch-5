#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/net.h>
#include <linux/in.h>
#include <linux/kthread.h>
#include <linux/slab.h>
#include <linux/socket.h>
#include <linux/tcp.h>
#include <linux/inet.h>   // for in4_pton()

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Modern Kernel TCP Client");

#define SERVER_IP "10.198.33.36" // replace with your server IP
#define SERVER_PORT 1036
#define BUF_SIZE 1024

static struct socket *client_socket = NULL;
static struct task_struct *client_thread;

static int client_fn(void *data)
{
    struct sockaddr_in server_addr;
    struct msghdr msg;
    struct kvec iov;
    char *buf;
    int ret;
    char *msg_to_send = "Hello from Kernel Client!";

    buf = kmalloc(BUF_SIZE, GFP_KERNEL);
    if (!buf)
        return -ENOMEM;

    ret = sock_create_kern(&init_net, AF_INET, SOCK_STREAM, IPPROTO_TCP, &client_socket);
    if (ret < 0)
    {
        printk(KERN_ERR "Client socket creation failed: %d\n", ret);
        kfree(buf);
        return ret;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    // Convert IP string to network format
    ret = in4_pton(SERVER_IP, -1, (u8 *)&server_addr.sin_addr.s_addr, -1, NULL);
    if (ret == 0)
    {
        printk(KERN_ERR "Invalid IP address: %s\n", SERVER_IP);
        sock_release(client_socket);
        kfree(buf);
        return -EINVAL;
    }

    ret = client_socket->ops->connect(client_socket,
                                      (struct sockaddr *)&server_addr,
                                      sizeof(server_addr), 0);
    if (ret < 0)
    {
        printk(KERN_ERR "Connect failed: %d\n", ret);
        sock_release(client_socket);
        kfree(buf);
        return ret;
    }

    printk(KERN_INFO "Connected to server\n");

    // Send message
    memset(&msg, 0, sizeof(msg));
    iov.iov_base = msg_to_send;
    iov.iov_len = strlen(msg_to_send);
    kernel_sendmsg(client_socket, &msg, &iov, 1, strlen(msg_to_send));
    printk(KERN_INFO "Message sent: %s\n", msg_to_send);

    // Receive response
    memset(&msg, 0, sizeof(msg));
    iov.iov_base = buf;
    iov.iov_len = BUF_SIZE;
    ret = kernel_recvmsg(client_socket, &msg, &iov, 1, BUF_SIZE, 0);
    if (ret > 0)
    {
        buf[ret] = '\0';
        printk(KERN_INFO "Received: %s\n", buf);
    }

    sock_release(client_socket);
    kfree(buf);
    printk(KERN_INFO "Client finished communication\n");
    return 0;
}

static int __init client_init(void)
{
    client_thread = kthread_run(client_fn, NULL, "kernel_tcp_client");
    if (IS_ERR(client_thread))
        return PTR_ERR(client_thread);
    return 0;
}

static void __exit client_exit(void)
{
    if (client_thread)
        kthread_stop(client_thread);
    if (client_socket)
        sock_release(client_socket);
    printk(KERN_INFO "Kernel TCP client unloaded\n");
}

module_init(client_init);
module_exit(client_exit);

