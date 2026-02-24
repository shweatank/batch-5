
static void __iomem *uart_base;
static void __iomem *led_base;

int val;

/* MMIO helpers */
#define uart_read(off)        readl(uart_base + (off))
#define uart_write(val, off)  writel((val), uart_base + (off))

/* ---------------- UART INIT ---------------- */
static void uart_hw_init(void)
{
    uart_write(0, UART_CR);        /* Disable UART */
    uart_write(0x7FF, UART_ICR);   /* Clear IRQs */

    /* 115200 baud @ 48MHz */
    uart_write(26, UART_IBRD);
    uart_write(3,  UART_FBRD);

    uart_write(LCRH_8BIT | LCRH_FEN, UART_LCRH);

    uart_write(CR_UARTEN | CR_TXE | CR_RXE, UART_CR);
    uart_write(0, UART_IMSC);      /* No interrupts */

    pr_info("rpi_uart: UART initialized\n");
}

/* ---------------- FILE OPS ---------------- */
static ssize_t uart_write_user(struct file *f,
                               const char __user *buf,
                               size_t len, loff_t *off)
{
    char ch,ch1;
    size_t i;

    for (i = 0; i < len; i++) {
        if (copy_from_user(&ch, buf + i, 1))
            return -EFAULT;
        while ((uart_read(UART_FR) & (1<<7))==0)//*FR_TXFF
            cpu_relax();
        uart_write(ch, UART_DR);
        printk("transmit is %c",ch);
      while((uart_read(UART_FR) & FR_RXFE))
              cpu_relax();
        ch1 = uart_read(UART_DR) & 0xFF;
        printk("recive is %c",ch1);
    }
    printk("\n");
    return len;
}

static ssize_t uart_read_user(struct file *f,
                              char __user *buf,
                              size_t len, loff_t *off)
{
    char ch;
    size_t i;

    for (i = 0; i < len; i++) {
       if (uart_read(UART_FR) & FR_RXFE)
       {
        break;}
        ch = uart_read(UART_DR) & 0xFF;
        printk("%c\n",ch);
        if (copy_to_user(buf + i, &ch, 1))
            return -EFAULT;
    }
    return i;

}

static struct file_operations uart_fops = {
    .owner = THIS_MODULE,
     .read  = uart_read_user,
    .write = uart_write_user,
};

/* ---------------- MODULE INIT ---------------- */
static int __init uart_init(void)
{
    int ret;

    uart_base = ioremap(UART_BASE, UART_SIZE);
    if (!uart_base) {
        pr_err("rpi_uart: ioremap failed\n");
        return -ENOMEM;
    }
    led_base = ioremap(LED_BASE,LED_SIZE);
    if(!led_base)
    {
            pr_err("led ioremap failed\n");
            return -ENOMEM;
    }

        val=readl(led_base+GPFSEL1);
        val=val&(~(7<<21));
//      val=val&(~(1<<21));

        val=val|(1<<21);
        writel(val,led_base+GPFSEL1);

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

    pr_info("rpi_uart: loaded (ioremap only)\n");
    return 0;
unregister:
    unregister_chrdev_region(devno, 1);
unmap:
    iounmap(uart_base);
    return ret;
}

/* ---------------- MODULE EXIT ---------------- */
static void __exit uart_exit(void)
{
    device_destroy(uart_class, devno);
    class_destroy(uart_class);
    cdev_del(&uart_cdev);
    unregister_chrdev_region(devno, 1);
    iounmap(uart_base);
    iounmap(led_base);
}
