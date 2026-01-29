#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#define PROC_NAME "proc_basic"
#define BUF_LEN   128

static char proc_buffer[BUF_LEN];
static size_t proc_buffer_len;

void reverse(char *str) {
	int len = strlen(str);
	for(int i = 0,j = len-1;i<j;i++,j--) {
		char temp = str[i];
		str[i] = str[j];
		str[j] = temp;
	}
}

void operation(char *str)
{
    int num1 = 0, num2 = 0, res = 0, i = 0, sign = 0;
    char op;

    while (str[i] >= '0' && str[i] <= '9') {
        num1 = num1 * 10 + (str[i] - '0');
        i++;
    }

    op = str[i];
    i++;

    while (str[i] >= '0' && str[i] <= '9') {
        num2 = num2 * 10 + (str[i] - '0');
        i++;
    }

    switch (op) {
        case '+': res = num1 + num2; break;
        case '-': res = num1 - num2; break;
        case '*': res = num1 * num2; break;
        case '/': res = (num2 != 0) ? num1 / num2 : 0; break;
        default:  return;
    }

    memset(str,0,strlen(str));

    i = 0;
    if (res < 0) {
        sign = 1;
        res = -res;
    }

    do {
        str[i++] = (res % 10) + '0';
        res /= 10;
    } while (res);

    if (sign)
        str[i++] = '-';

    str[i] = '\0';

    reverse(str);

}


static ssize_t proc_read(struct file *file,
                         char __user *user_buf,
                         size_t count,
                         loff_t *ppos)
{
	//reverse(proc_buffer);
	operation(proc_buffer);
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
MODULE_AUTHOR("TechDhaba");
MODULE_DESCRIPTION("Basic procfs driver example");
