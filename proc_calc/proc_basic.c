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
    return simple_read_from_buffer(user_buf,
                                   count,
                                   ppos,
                                   proc_buffer,
                                   proc_buffer_len);
}

static ssize_t proc_write(struct file *file,
                          const char __user *user_buf,
                          size_t count,
                          loff_t *ppos)
{
    char op[4];
    int a, b, result;

    if (count > BUF_LEN - 1)
        count = BUF_LEN - 1;

    if (copy_from_user(proc_buffer, user_buf, count))
        return -EFAULT;

    proc_buffer[count] = '\0';

    /* Parse input: op a b */
    if (sscanf(proc_buffer, "%3s %d %d", op, &a, &b) != 3) {
        proc_buffer_len = snprintf(proc_buffer, BUF_LEN,
                                   "Invalid format. Use: add|sub|mul|div a b\n");
        return count;
    }

    if (strcmp(op, "add") == 0)
        result = a + b;
    else if (strcmp(op, "sub") == 0)
        result = a - b;
    else if (strcmp(op, "mul") == 0)
        result = a * b;
    else if (strcmp(op, "div") == 0) {
        if (b == 0) {
            proc_buffer_len = snprintf(proc_buffer, BUF_LEN,
                                       "Error: Division by zero\n");
            return count;
        }
        result = a / b;
    } else {
        proc_buffer_len = snprintf(proc_buffer, BUF_LEN,
                                   "Unknown operation\n");
        return count;
    }

    proc_buffer_len = snprintf(proc_buffer, BUF_LEN,
                               "Result: %d\n", result);

    pr_info("proc_basic: %s %d %d = %d\n", op, a, b, result);

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
MODULE_DESCRIPTION("Procfs calculator driver");

