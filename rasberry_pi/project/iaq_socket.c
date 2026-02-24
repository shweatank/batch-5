#include "iaq_internal.h"
#include <linux/net.h>
#include <net/sock.h>
#include <linux/in.h>

int iaq_socket_init(void)
{
    int ret;

    ret = sock_create_kern(&init_net,
                           AF_INET,
                           SOCK_STREAM,
                           IPPROTO_TCP,
                           &iaq->alert_sock);
    if (ret < 0) {
        pr_err("Socket creation failed\n");
        return ret;
    }

    iaq->server_addr.sin_family = AF_INET;
    iaq->server_addr.sin_port = htons(9000);

    /* 10.25.109.67 */
    iaq->server_addr.sin_addr.s_addr =
        htonl((10 << 24) |
              (25 << 16) |
              (109 << 8) |
              67);

    ret = kernel_connect(iaq->alert_sock,
            (struct sockaddr *)&iaq->server_addr,
            sizeof(iaq->server_addr), 0);

    if (ret < 0)
        pr_err("Socket connect failed\n");
    else
        pr_info("Connected to PC\n");

    return ret;
}

void iaq_socket_cleanup(void)
{
    if (iaq->alert_sock)
        sock_release(iaq->alert_sock);
}

