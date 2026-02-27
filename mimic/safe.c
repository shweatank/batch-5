#include <linux/module.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/kthread.h>
#include <linux/delay.h>

#define UART_BASE  0xFE201000
#define UART_SIZE  0x100

#define UART_DR    0x00
#define UART_FR    0x18
#define UART_IBRD  0x24
#define UART_FBRD  0x28
#define UART_LCRH  0x2C
#define UART_CR    0x30
#define UART_ICR   0x44

#define FR_TXFF (1 << 5)
#define FR_RXFE (1 << 4)

#define CR_UARTEN (1 << 0)
#define CR_TXE    (1 << 8)
#define CR_RXE    (1 << 9)

#define LCRH_8BIT (3 << 5)
#define LCRH_FEN  (1 << 4)

static void __iomem *uart_base;
static struct task_struct *mimic_thread;
static int registered = 0;

/* ===== GC07 ACK PACKETS ===== */

static u8 success_ack[] = {
    0xEF,0x01,0xFF,0xFF,0xFF,0xFF,
    0x07,0x00,0x03,
    0x00,
    0x00,0x0A
};

static u8 fail_ack[] = {
    0xEF,0x01,0xFF,0xFF,0xFF,0xFF,
    0x07,0x00,0x03,
    0x01,
    0x00,0x0B
};

/* ================= UART INIT ================= */

static void uart_hw_init(void)
{
    writel(0, uart_base + UART_CR);
    writel(0x7FF, uart_base + UART_ICR);

    writel(26, uart_base + UART_IBRD);
    writel(3,  uart_base + UART_FBRD);

    writel(LCRH_8BIT | LCRH_FEN, uart_base + UART_LCRH);
    writel(CR_UARTEN | CR_TXE | CR_RXE, uart_base + UART_CR);

    pr_info("MIMIC: UART initialized\n");
}

/* ================= UART HELPERS ================= */

static void uart_write_byte(u8 byte)
{
    while (readl(uart_base + UART_FR) & FR_TXFF)
        cpu_relax();

    writel(byte, uart_base + UART_DR);
}

static int uart_read_byte(u8 *byte)
{
    if (readl(uart_base + UART_FR) & FR_RXFE)
        return 0;

    *byte = readl(uart_base + UART_DR) & 0xFF;
    return 1;
}

static void uart_send(u8 *data, int len)
{
    int i;
    for (i = 0; i < len; i++)
        uart_write_byte(data[i]);
}

/* ================= MIMIC THREAD ================= */

static int mimic_fn(void *data)
{
    u8 buffer[32];
    int index = 0;

    pr_info("GC07 Mimic Started...\n");

    while (!kthread_should_stop())
    {
        u8 byte;

        if (uart_read_byte(&byte))
        {
		// Start-of-packet check
    		if (index == 0 && byte != 0xEF){
        	continue;  // discard any garbage
		}
            buffer[index++] = byte;
	//pr_info("byte :%d  0x%02X   ",index-1,buffer[index-1]);
            if (index >= 12)  // minimum GC07 packet
            {
                pr_info("MIMIC: Packet received, CMD=0x%02X\n", buffer[9]);

                switch (buffer[9])
                {
                    case 0x01:  // GET_IMAGE
                        pr_info("MIMIC: GET_IMAGE\n");
                        uart_send(success_ack, sizeof(success_ack));
                        break;

                    case 0x06:  // FP_ADD
                        pr_info("MIMIC: FP_ADD (Register)\n");
                        registered = 1;
                        uart_send(success_ack, sizeof(success_ack));
                        break;

                    case 0x04:  // FP_SEARCH
                        pr_info("MIMIC: FP_SEARCH\n");
                        if (registered)
                            uart_send(success_ack, sizeof(success_ack));
                        else
                            uart_send(fail_ack, sizeof(fail_ack));
                        break;

                    default:
                        pr_info("MIMIC: Unknown CMD\n");
                        break;
                }

                index = 0;
		memset(buffer,0,sizeof(buffer));
            }
        }

        msleep(5);
    }

    return 0;
}

/* ================= MODULE INIT ================= */

static int __init mimic_init(void)
{
    uart_base = ioremap(UART_BASE, UART_SIZE);
    if (!uart_base)
        return -ENOMEM;

    uart_hw_init();

    mimic_thread = kthread_run(mimic_fn, NULL, "gc07_mimic");
    if (IS_ERR(mimic_thread))
        return PTR_ERR(mimic_thread);

    pr_info("GC07 Mimic Module Loaded\n");
    return 0;
}

/* ================= MODULE EXIT ================= */

static void __exit mimic_exit(void)
{
    if (mimic_thread)
        kthread_stop(mimic_thread);

    if (uart_base)
        iounmap(uart_base);

    pr_info("GC07 Mimic Module Unloaded\n");
}

module_init(mimic_init);
module_exit(mimic_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("GC07 Kernel Mimic");
MODULE_DESCRIPTION("Kernel GC07 Fingerprint Sensor Emulator using PL011 MMIO");
