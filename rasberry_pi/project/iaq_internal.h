#ifndef IAQ_INTERNAL_H
#define IAQ_INTERNAL_H

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <linux/workqueue.h>
#include <linux/interrupt.h>
#include <linux/kthread.h>
#include <linux/wait.h>
#include <linux/spinlock.h>
#include <linux/mutex.h>
#include <linux/of.h>
#include <linux/slab.h>
#include <linux/net.h>
#include <net/sock.h>
#include <linux/in.h>

#define IAQ_BUF_SIZE 64

struct iaq_sample {
    u32 value;
    ktime_t timestamp;
};

struct iaq_dev {
    struct hrtimer timer;
    ktime_t interval;

    struct tasklet_struct tasklet;
    struct work_struct work;

    struct task_struct *thread;

    struct iaq_sample buffer[IAQ_BUF_SIZE];
    int head;

    spinlock_t buf_lock;
    struct mutex data_lock;

    wait_queue_head_t wq;
    atomic_t data_ready;

    u32 threshold;

    struct task_struct *alert_thread;

    struct socket *alert_sock;
    struct sockaddr_in server_addr;

    wait_queue_head_t alert_wq;
    atomic_t alert_flag;
};

extern struct iaq_dev *iaq;

/* timer */
void iaq_timer_init(void);
void iaq_timer_cleanup(void);

/* worker */
void iaq_worker_init(void);
void iaq_worker_cleanup(void);

/* lcd thread */
void iaq_thread_init(void);
void iaq_thread_cleanup(void);

/* alert thread */
void iaq_alert_thread_init(void);
void iaq_alert_thread_cleanup(void);

/* socket */
int iaq_socket_init(void);
void iaq_socket_cleanup(void);

/* dts */
void iaq_parse_dts(void);

void iaq_buffer_push(struct iaq_sample *sample);

#endif

