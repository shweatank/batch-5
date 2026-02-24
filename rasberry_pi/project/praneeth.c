#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <linux/workqueue.h>
#include <linux/kthread.h>
#include <linux/wait.h>
#include <linux/spinlock.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/net.h>
#include <net/sock.h>
#include <linux/in.h>

#define IAQ_BUF_SIZE 64                 // Circular buffer size for IAQ samples
#define SERVER_IP   ((10<<24)|(25<<16)|(109<<8)|67) // Remote server IP
#define SERVER_PORT 9000                // Remote server port

/* ===================== DATA STRUCTURES ===================== */

/* Structure representing one IAQ sample */
struct iaq_sample {
    u32 value;          // Air quality value
    ktime_t timestamp;  // Time of sample
};

/* Main device structure */
struct iaq_dev {
    struct hrtimer timer;        // High-resolution timer
    ktime_t interval;            // Timer interval

    struct work_struct work;     // Workqueue item

    struct task_struct *thread;        // Thread for printing values
    struct task_struct *alert_thread;  // Thread for sending alerts

    struct iaq_sample buffer[IAQ_BUF_SIZE]; // Circular buffer of samples
    int head;                             // Index for next sample

    spinlock_t buf_lock;     // Protect buffer in interrupt/softirq context
    struct mutex data_lock;  // Protect shared data in process context

    wait_queue_head_t wq;       // Wait queue for data ready
    wait_queue_head_t alert_wq; // Wait queue for alert flag

    atomic_t data_ready;   // Flag indicating new data
    atomic_t alert_flag;   // Flag indicating alert needs sending

    u32 threshold;         // Threshold for alert

    struct socket *alert_sock;       // Kernel socket to send alert
    struct sockaddr_in server_addr;  // Server address
};

/* Global device instance */
static struct iaq_dev *iaq;

/* Default threshold initialized globally */
static const u32 IAQ_DEFAULT_THRESHOLD = 1000;

/* ===================== BUFFER FUNCTIONS ===================== */

/* Push sample into circular buffer */
static void iaq_buffer_push(struct iaq_sample *sample)
{
    spin_lock(&iaq->buf_lock);         // Protect buffer from concurrent access

    iaq->buffer[iaq->head++] = *sample; // Store sample
    iaq->head %= IAQ_BUF_SIZE;          // Wrap around for circular buffer

    spin_unlock(&iaq->buf_lock);
}

/* ===================== SOCKET FUNCTIONS ===================== */

/* Initialize kernel TCP socket to remote server */
static int iaq_socket_init(void)
{
    int ret;

    ret = sock_create_kern(&init_net,
                           AF_INET,
                           SOCK_STREAM,
                           IPPROTO_TCP,
                           &iaq->alert_sock);
    if (ret < 0)
        return ret;

    iaq->server_addr.sin_family = AF_INET;
    iaq->server_addr.sin_port = htons(SERVER_PORT);
    iaq->server_addr.sin_addr.s_addr = htonl(SERVER_IP);

    ret = kernel_connect(iaq->alert_sock,
                         (struct sockaddr *)&iaq->server_addr,
                         sizeof(iaq->server_addr), 0);

    if (ret < 0)
        pr_err("Socket connect failed\n");
    else
        pr_info("Connected to PC\n");

    return ret;
}

/* Cleanup socket */
static void iaq_socket_cleanup(void)
{
    if (iaq->alert_sock)
        sock_release(iaq->alert_sock);
}

/* ===================== WORKQUEUE FUNCTIONS ===================== */

/* Worker function executes in process context, safe for blocking ops */
static void iaq_work_fn(struct work_struct *work)
{
    struct iaq_sample sample;

    /* Protect sample creation */
    mutex_lock(&iaq->data_lock);

    sample.value = 750 + (jiffies % 500);   // Simulated IAQ value
    sample.timestamp = ktime_get();

    mutex_unlock(&iaq->data_lock);

    iaq_buffer_push(&sample);                // Push to circular buffer

    /* Notify IAQ thread that new data is ready */
    atomic_set(&iaq->data_ready, 1);
    wake_up(&iaq->wq);

    /* Trigger alert if threshold exceeded */
    if (sample.value > iaq->threshold) {
        atomic_set(&iaq->alert_flag, 1);
        wake_up(&iaq->alert_wq);
    }
}

/* Initialize workqueue item */
static void iaq_worker_init(void)
{
    INIT_WORK(&iaq->work, iaq_work_fn);
}

/* Cleanup workqueue */
static void iaq_worker_cleanup(void)
{
    cancel_work_sync(&iaq->work);
}

/* ===================== TIMER FUNCTIONS ===================== */

/* Timer callback: schedules workqueue */
static enum hrtimer_restart iaq_timer_cb(struct hrtimer *t)
{
    schedule_work(&iaq->work);           // Defer heavy work to process context
    hrtimer_forward_now(t, iaq->interval); // Restart timer
    return HRTIMER_RESTART;
}

/* Initialize high-resolution timer */
static void iaq_timer_init(void)
{
    iaq->interval = ms_to_ktime(5000);   // 5-second interval

    hrtimer_init(&iaq->timer,
                 CLOCK_MONOTONIC,
                 HRTIMER_MODE_REL);

    iaq->timer.function = iaq_timer_cb;

    hrtimer_start(&iaq->timer,
                  iaq->interval,
                  HRTIMER_MODE_REL);
}

/* Cleanup timer */
static void iaq_timer_cleanup(void)
{
    hrtimer_cancel(&iaq->timer);
}

/* ===================== THREAD FUNCTIONS ===================== */

/* Thread to print IAQ values */
static int iaq_thread_fn(void *data)
{
    while (!kthread_should_stop()) {

        /* Sleep until new data is ready */
        wait_event_interruptible(iaq->wq,
            atomic_read(&iaq->data_ready));

        atomic_set(&iaq->data_ready, 0);

        /* Get latest sample safely */
        spin_lock(&iaq->buf_lock);
        struct iaq_sample latest =
            iaq->buffer[(iaq->head - 1 + IAQ_BUF_SIZE)
                        % IAQ_BUF_SIZE];
        spin_unlock(&iaq->buf_lock);

        pr_info("IAQ Value: %u\n", latest.value);

        if (latest.value > iaq->threshold)
            pr_warn("!!! ALERT TRIGGERED !!!\n");
    }
    return 0;
}

/* Thread to send alert via TCP */
static int alert_thread_fn(void *data)
{
    struct msghdr msg = {};
    struct kvec iov;
    int ret;
    u32 alert_data = 1;

    while (!kthread_should_stop()) {

        /* Sleep until alert flag is set */
        wait_event_interruptible(iaq->alert_wq,
            atomic_read(&iaq->alert_flag));

        atomic_set(&iaq->alert_flag, 0);

        /* Prepare and send alert message */
        iov.iov_base = &alert_data;
        iov.iov_len  = sizeof(alert_data);

        ret = kernel_sendmsg(iaq->alert_sock,
                             &msg,
                             &iov,
                             1,
                             sizeof(alert_data));

        if (ret > 0)
            pr_info("Alert sent\n");
    }
    return 0;
}

/* Initialize both kernel threads */
static void iaq_threads_init(void)
{
    iaq->thread =
        kthread_run(iaq_thread_fn,
                    NULL,
                    "iaq_thread");

    iaq->alert_thread =
        kthread_run(alert_thread_fn,
                    NULL,
                    "iaq_alert_thread");
}

/* Stop threads during cleanup */
static void iaq_threads_cleanup(void)
{
    if (iaq->thread)
        kthread_stop(iaq->thread);

    if (iaq->alert_thread)
        kthread_stop(iaq->alert_thread);
}

/* ===================== MODULE INIT/EXIT ===================== */

/* Module initialization */
static int __init iaq_init(void)
{
    iaq = kzalloc(sizeof(*iaq), GFP_KERNEL);  // Allocate device struct
    if (!iaq)
        return -ENOMEM;

    /* Initialize locks and wait queues */
    spin_lock_init(&iaq->buf_lock);
    mutex_init(&iaq->data_lock);
    init_waitqueue_head(&iaq->wq);
    init_waitqueue_head(&iaq->alert_wq);

    atomic_set(&iaq->data_ready, 0);
    atomic_set(&iaq->alert_flag, 0);

    iaq->head = 0;
    iaq->threshold = IAQ_DEFAULT_THRESHOLD; // Set threshold globally

    iaq_worker_init();     // Init workqueue
    iaq_socket_init();     // Init kernel socket
    iaq_threads_init();    // Start threads
    iaq_timer_init();      // Start timer

    pr_info("IAQ Driver Loaded\n");
    return 0;
}

/* Module cleanup */
static void __exit iaq_exit(void)
{
    iaq_timer_cleanup();      // Stop timer
    iaq_worker_cleanup();     // Cancel work
    iaq_threads_cleanup();    // Stop threads
    iaq_socket_cleanup();     // Release socket
    kfree(iaq);               // Free memory

    pr_info("IAQ Driver Unloaded\n");
}

module_init(iaq_init);
module_exit(iaq_exit);
MODULE_LICENSE("GPL");
