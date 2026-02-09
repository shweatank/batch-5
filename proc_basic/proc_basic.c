#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#define PROC_NAME "proc_basic"
#define BUF_LEN   128

static char proc_buffer[BUF_LEN];
static size_t proc_buffer_len;

static ssize_t proc_read(struct file *file,
                         char __user *user_buf,
                         size_t count,
                         loff_t *ppos)
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

static ssize_t proc_write(struct file *file,
                          const char __user *user_buf,
                          size_t count,
                          loff_t *ppos)
{
    if (count > BUF_LEN - 1)
        count = BUF_LEN - 1;

    if (copy_from_user(proc_buffer, user_buf, count))
        return -EFAULT;

    proc_buffer[count] = '\0';
    proc_buffer_len = count;

    pr_info("proc_basic: received \"%s\"\n", proc_buffer);
    return count;
}

static const struct proc_ops proc_fops = {
    .proc_read  = proc_read,
    .proc_write = proc_write,
};

static int __init proc_basic_init(void)
{
    proc_create(PROC_NAME, 0666, NULL, &proc_fops);
    pr_info("proc_basic: loaded\n");
    return 0;
}

static void __exit proc_basic_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
    pr_info("proc_basic: unloaded\n");
}

module_init(proc_basic_init);
module_exit(proc_basic_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abhi");
MODULE_DESCRIPTION("Basic procfs driver with copy_to_user");
