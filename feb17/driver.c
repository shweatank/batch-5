// SPDX-License-Identifier: GPL
#include <linux/module.h>
#include <linux/hrtimer.h>
#include <linux/kthread.h>
#include <linux/workqueue.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/random.h>

#define DRIVER_NAME "sgp30_oled_stub"
#define READ_INTERVAL 5  // seconds
#define ECO2_THRESHOLD 1000

static struct hrtimer timer;
static struct task_struct *thread;
static struct workqueue_struct *wq;
static struct work_struct oled_work;
static DEFINE_MUTEX(data_lock);

static u16 eco2_prev, tvoc_prev;
static u16 eco2_curr, tvoc_curr;
static int alert_flag;

// ---------------- OLED STUB ----------------

static void oled_cmd(u8 cmd)   { pr_info("OLED CMD: 0x%02x\n", cmd); }
static void oled_data(u8 data) { pr_info("OLED DATA: 0x%02x\n", data); }
static void oled_clear(void)   { pr_info("OLED CLEAR\n"); }
static void oled_print_char(char c) { pr_info("OLED CHAR: %c\n", c); }

static void oled_print_string_kernel(char *str)
{
    pr_info("OLED LINE: %s\n", str);
}

// ---------------- SGP30 STUB ----------------

static int sgp30_read(void)
{
    static u16 fake_eco2 = 400;
    static u16 fake_tvoc = 0;

    // Increment readings to simulate change
    fake_eco2 += 50;
    if (fake_eco2 > 2000) fake_eco2 = 400;

    fake_tvoc += 10;
    if (fake_tvoc > 600) fake_tvoc = 0;

    eco2_curr = fake_eco2;
    tvoc_curr = fake_tvoc;

    alert_flag = (eco2_curr > ECO2_THRESHOLD) ? 1 : 0;

    pr_info("SGP30_STUB: eCO2=%d, TVOC=%d, ALERT=%d\n",
            eco2_curr, tvoc_curr, alert_flag);

    return 0;
}

// ---------------- WORKQUEUE ----------------

static void oled_work_handler(struct work_struct *work)
{
    char buf[64];

    oled_clear();

    snprintf(buf, sizeof(buf), "eCO2:%d ppm", eco2_curr);
    oled_print_string_kernel(buf);

    snprintf(buf, sizeof(buf), "TVOC:%d ppb", tvoc_curr);
    oled_print_string_kernel(buf);

    if (alert_flag)
        oled_print_string_kernel("ALERT!");
}

// ---------------- KTHREAD ----------------

static int thread_fn(void *data)
{
    while (!kthread_should_stop()) {
        if (sgp30_read() == 0) {
            mutex_lock(&data_lock);

            if (eco2_curr != eco2_prev || tvoc_curr != tvoc_prev) {
                eco2_prev = eco2_curr;
                tvoc_prev = tvoc_curr;
                queue_work(wq, &oled_work);
            }

            mutex_unlock(&data_lock);
        }

        set_current_state(TASK_INTERRUPTIBLE);
        schedule();
    }
    return 0;
}

// ---------------- HRTIMER ----------------

static enum hrtimer_restart timer_callback(struct hrtimer *t)
{
    wake_up_process(thread);
    hrtimer_forward_now(&timer, ktime_set(READ_INTERVAL, 0));
    return HRTIMER_RESTART;
}

// ---------------- MODULE INIT / EXIT ----------------

static int __init stub_init(void)
{
    pr_info("SGP30 + OLED stub driver loaded\n");

    wq = create_singlethread_workqueue("oled_wq");
    INIT_WORK(&oled_work, oled_work_handler);

    thread = kthread_run(thread_fn, NULL, "sgp30_thread");

    hrtimer_init(&timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    timer.function = timer_callback;
    hrtimer_start(&timer, ktime_set(READ_INTERVAL, 0), HRTIMER_MODE_REL);

    return 0;
}

static void __exit stub_exit(void)
{
    hrtimer_cancel(&timer);
    kthread_stop(thread);
    destroy_workqueue(wq);

    pr_info("SGP30 + OLED stub driver removed\n");
}

module_init(stub_init);
module_exit(stub_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("SGP30 + OLED kernel driver stub for x86 testing without hardware");

