#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>

#define PROC_NAME "proc_basic"
#define BUF_LEN   128

static char proc_buffer[BUF_LEN];
static size_t proc_buffer_len;


static struct workqueue_struct *my_wq;
static struct work_struct my_work;


static int irq_line = 1;


static void my_work_fn(struct work_struct *work)
{
    pr_info("proc_irq_wq: Workqueue running, buffer = \"%s\"\n", proc_buffer);
}


static irqreturn_t my_irq_handler(int irq, void *dev_id)
{
    pr_info("proc_irq_wq: Interrupt received on IRQ %d\n", irq);

    
    queue_work(my_wq, &my_work);

    return IRQ_HANDLED;
}


static ssize_t proc_read(struct file *file, char __user *user_buf,
                         size_t count, loff_t *ppos)
{
    size_t available;

    if (*ppos > 0)
        return 0;

    available = proc_buffer_len;
    if (count < available)
        available = count;

    if (copy_to_user(user_buf, proc_buffer, available))
        return -EFAULT;

    *ppos += available;
    return available;
}


static ssize_t proc_write(struct file *file, const char __user *user_buf,
                          size_t count, loff_t *ppos)
{
    if (count > BUF_LEN - 1)
        count = BUF_LEN - 1;

    if (copy_from_user(proc_buffer, user_buf, count))
        return -EFAULT;

    proc_buffer[count] = '\0';
    proc_buffer_len = count;

    pr_info("proc_irq_wq: Received \"%s\"\n", proc_buffer);

    return count;
}

static const struct proc_ops proc_fops = {
    .proc_read  = proc_read,
    .proc_write = proc_write,
};


static int __init proc_irq_wq_init(void)
{
   
    if (!proc_create(PROC_NAME, 0666, NULL, &proc_fops)) {
        pr_err("proc_irq_wq: Failed to create /proc/%s\n", PROC_NAME);
        return -ENOMEM;
    }

    
    my_wq = create_singlethread_workqueue("proc_irq_wq");
    if (!my_wq) {
        remove_proc_entry(PROC_NAME, NULL);
        pr_err("proc_irq_wq: Failed to create workqueue\n");
        return -ENOMEM;
    }
    INIT_WORK(&my_work, my_work_fn);

    
    if (request_irq(irq_line, my_irq_handler, IRQF_SHARED, "proc_irq_wq", (void *)my_irq_handler)) {
        destroy_workqueue(my_wq);
        remove_proc_entry(PROC_NAME, NULL);
        pr_err("proc_irq_wq: Failed to request IRQ %d\n", irq_line);
        return -EBUSY;
    }

    pr_info("proc_irq_wq: Module loaded (IRQ + workqueue)\n");
    return 0;
}


static void __exit proc_irq_wq_exit(void)
{
    free_irq(irq_line, (void *)my_irq_handler);

    remove_proc_entry(PROC_NAME, NULL);

    if (my_wq) {
        flush_workqueue(my_wq);
        destroy_workqueue(my_wq);
    }

    pr_info("proc_irq_wq: Module unloaded\n");
}

module_init(proc_irq_wq_init);
module_exit(proc_irq_wq_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abhi");
MODULE_DESCRIPTION("procfs module with IRQ-driven workqueue");

