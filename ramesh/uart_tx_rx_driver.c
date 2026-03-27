/* ================== LOW-LEVEL RX ================== */

static int uart_getc(char *c)
{
    u32 dr;

    while (uart_read(UART_FR) & FR_RXFE)
        cpu_relax();

    dr = uart_read(UART_DR);

    /* RX error? clear and retry */
    if (dr & 0xF00) {
        uart_write(0x7FF, UART_ECR);
        return -EIO;
    }

    *c = dr & 0xFF;
    return 0;
}

/* ================== FILE OPS ================== */

static ssize_t uart_write_user(struct file *f,
                               const char __user *buf,
                               size_t len, loff_t *off)
{
    size_t i;
    char c;

    for (i = 0; i < len; i++) {
        if (copy_from_user(&c, buf + i, 1))
            return -EFAULT;

        uart_putc(c);
        printk("data sent : %c\n",c);
//      uart_getc(&c);
//      printk("data received : %c\n",c);
    }

    return len;
}

static ssize_t uart_read_user(struct file *f,
                              char __user *buf,
                              size_t len, loff_t *off)
{
    size_t i;
    char c;

    for (i = 0; i < len; i++) {
        if (uart_getc(&c))
            break;

        if (copy_to_user(buf + i, &c, 1))
            return -EFAULT;
        printk("data received : %c\n",c);
    }

    return i;
}

static struct file_operations uart_fops = {
    .owner = THIS_MODULE,
    .read  = uart_read_user,
    .write = uart_write_user,
};

/* ================== MODULE INIT ================== */

static int __init uart_init(void)
{
    int ret;

    uart_base = ioremap(UART_BASE_PHYS, UART_SIZE);
    if (!uart_base)
        return -ENOMEM;

    ret = alloc_chrdev_region(&devno, 0, 1, DEVICE_NAME);
    if (ret)
        goto unmap;

    cdev_init(&uart_cdev, &uart_fops);
    ret = cdev_add(&uart_cdev, devno, 1);
    if (ret)
        goto unregister;

    uart_class = class_create(DEVICE_NAME);
    device_create(uart_class, NULL, devno, NULL, DEVICE_NAME);

    uart_hw_init();

    pr_info("rpi_uart: driver loaded\n");
    return 0;

unregister:
    unregister_chrdev_region(devno, 1);
unmap:
    iounmap(uart_base);
    return ret;
}

/* ================== MODULE EXIT ================== */

static void __exit uart_exit(void)
{
    device_destroy(uart_class, devno);
    class_destroy(uart_class);
    cdev_del(&uart_cdev);
    unregister_chrdev_region(devno, 1);
    iounmap(uart_base);
    pr_info("rpi_uart: driver unloaded\n");
}

module_init(uart_init);
module_exit(uart_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("You");
MODULE_DESCRIPTION("Raspberry Pi PL011 UART driver using ioremap");
