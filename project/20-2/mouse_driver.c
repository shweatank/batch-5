#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/workqueue.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define GPIO_BASE 0xFE200000
#define GPFSEL1 0x04
#define GPSET0  0x1C
#define GPCLR0  0x28
#define LED_GPIO 17

#define UART0_BASE 0xFE201000
#define UART_FR    0x18
#define UART_DR    0x00
#define UART_FR_TXFF (1 << 5)
#define UART_CR   0x30
#define UART_CR_UARTEN (1 << 0)
#define UART_CR_TXE   (1 << 8)

#define DEVICE_NAME "mouse_touch"
#define MAX_NAME_LEN 64

/* Globals */
static void __iomem *gpio_base;
static void __iomem *uart_base;

static struct input_handle *mouse_handle;
static int x_pos = SCREEN_WIDTH/2;
static int y_pos = SCREEN_HEIGHT/2;

static struct workqueue_struct *ili_wq;
static struct work_struct ili_work;
static char ili_string[MAX_NAME_LEN] = "DEFAULT";

static char stored_name[MAX_NAME_LEN] = "DEFAULT";
static int major_num;

static bool mouse_connected = false;
static bool uart_ready = false;

/* GPIO helpers */
static void gpio17_set_output(void)
{
	u32 val = ioread32(gpio_base + GPFSEL1);
	val &= ~(0x7 << 21);
	val |=  (0x1 << 21);
	iowrite32(val, gpio_base + GPFSEL1);
}

static void led_on(void)  { iowrite32(1 << LED_GPIO, gpio_base + GPSET0); }
static void led_off(void) { iowrite32(1 << LED_GPIO, gpio_base + GPCLR0); }

/* UART helpers */
static void uart_send_char(char c)
{
	if(!uart_base) return;
	while (ioread32(uart_base + UART_FR) & UART_FR_TXFF)
		cpu_relax();
	iowrite32(c, uart_base + UART_DR);
}

static void uart_send_string(const char *str)
{
	while(*str)
		uart_send_char(*str++);
}

/* Workqueue function for ILI9225 (signal userspace) */
static void ili_work_func(struct work_struct *work)
{
	/* This simply keeps the string updated in kernel; userspace reads via /dev/mouse_touch */
	pr_info("ILI9225 Workqueue triggered: %s\n", ili_string);
}

/* Mouse event handler */
static void mouse_event(struct input_handle *handle,
		unsigned int type,
		unsigned int code,
		int value)
{
	if(!mouse_connected)
		return;

	if(type == EV_REL){
		if(code == REL_X){
			x_pos += value;
			if(x_pos < 0) x_pos = 0;
			if(x_pos > SCREEN_WIDTH) x_pos = SCREEN_WIDTH;
		}
		if(code == REL_Y){
			y_pos += value;
			if(y_pos < 0) y_pos = 0;
			if(y_pos > SCREEN_HEIGHT) y_pos = SCREEN_HEIGHT;
		}
	}

	if(type == EV_KEY && code == BTN_LEFT && value == 1){
		if(x_pos > SCREEN_WIDTH/2 && y_pos < SCREEN_HEIGHT/2){
			pr_info("RIGHT TOP → LED ON\n");
			led_on();
		} else if(x_pos > SCREEN_WIDTH/2 && y_pos > SCREEN_HEIGHT/2){
			pr_info("RIGHT BOTTOM → Send string to ILI9225\n");
			strncpy(ili_string, stored_name, MAX_NAME_LEN-1);
			queue_work(ili_wq, &ili_work);
		} else if(x_pos < SCREEN_WIDTH/2 && y_pos < SCREEN_HEIGHT/2){
			pr_info("LEFT TOP → Sending string via UART: %s\n", stored_name);
			uart_send_string(stored_name);
			uart_send_string("\n");
		} else {
			pr_info("LEFT BOTTOM → LED OFF\n");
			led_off();
		}
	}
}

/* Mouse connect/disconnect */
static int mouse_connect(struct input_handler *handler,
		struct input_dev *dev,
		const struct input_device_id *id)
{
	mouse_handle = kzalloc(sizeof(*mouse_handle), GFP_KERNEL);
	if(!mouse_handle) return -ENOMEM;

	mouse_handle->dev = dev;
	mouse_handle->handler = handler;
	mouse_handle->name = "mouse_touch_handle";

	if(input_register_handle(mouse_handle)){
		kfree(mouse_handle);
		return -EINVAL;
	}

	if(input_open_device(mouse_handle)){
		input_unregister_handle(mouse_handle);
		kfree(mouse_handle);
		return -EINVAL;
	}

	mouse_connected = true;
	pr_info("Mouse connected\n");
	return 0;
}

static void mouse_disconnect(struct input_handle *handle)
{
	input_close_device(handle);
	input_unregister_handle(handle);
	kfree(handle);
	mouse_connected = false;
}

/* Input ID table */
static const struct input_device_id mouse_ids[] = {
	{
		.flags = INPUT_DEVICE_ID_MATCH_EVBIT,
		.evbit = { BIT_MASK(EV_KEY) | BIT_MASK(EV_REL) },
	},
	{}
};
MODULE_DEVICE_TABLE(input, mouse_ids);

/* Input handler */
static struct input_handler mouse_handler = {
	.event = mouse_event,
	.connect = mouse_connect,
	.disconnect = mouse_disconnect,
	.name = "mouse_touch_handler",
	.id_table = mouse_ids,
};

/* Chardev write */
static ssize_t mouse_touch_write(struct file *file,
		const char __user *buf,
		size_t count, loff_t *ppos)
{
	if(count >= MAX_NAME_LEN) count = MAX_NAME_LEN-1;
	if(copy_from_user(stored_name, buf, count))
		return -EFAULT;
	stored_name[count] = '\0';
	pr_info("Chardev: stored name = %s\n", stored_name);
	return count;
}

/* File operations */
static const struct file_operations fops = {
	.owner = THIS_MODULE,
	.write = mouse_touch_write,
};

/* Module init/exit */
static int __init touch_driver_init(void)
{
	pr_info("Mouse Touch + LED + UART + ILI9225 Driver Loading...\n");

	/* GPIO */
	gpio_base = ioremap(GPIO_BASE, 0xB4);
	if(!gpio_base){
		pr_err("Failed ioremap GPIO\n");
		return -ENOMEM;
	}
	gpio17_set_output();
	led_off();

	/* UART */
	uart_base = ioremap(UART0_BASE, 0x1000);
	if(!uart_base){
		iounmap(gpio_base);
		return -ENOMEM;
	}
	iowrite32(UART_CR_UARTEN | UART_CR_TXE, uart_base + UART_CR);
	uart_ready = true;

	/* Workqueue for ILI9225 */
	ili_wq = create_singlethread_workqueue("ili_wq");
	if(!ili_wq){
		iounmap(gpio_base);
		iounmap(uart_base);
		return -ENOMEM;
	}
	INIT_WORK(&ili_work, ili_work_func);

	/* Input handler */
	input_register_handler(&mouse_handler);

	/* Chardev */
	major_num = register_chrdev(0, DEVICE_NAME, &fops);
	if(major_num < 0){
		input_unregister_handler(&mouse_handler);
		destroy_workqueue(ili_wq);
		iounmap(gpio_base);
		iounmap(uart_base);
		return major_num;
	}

	pr_info("Driver loaded, major=%d\n", major_num);
	return 0;
}

static void __exit touch_driver_exit(void)
{
	unregister_chrdev(major_num, DEVICE_NAME);
	input_unregister_handler(&mouse_handler);

	flush_workqueue(ili_wq);
	destroy_workqueue(ili_wq);

	led_off();
	if(gpio_base) iounmap(gpio_base);
	if(uart_base) iounmap(uart_base);

	pr_info("Driver unloaded\n");
}

module_init(touch_driver_init);
module_exit(touch_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TEAM 2 PROJECT");
MODULE_DESCRIPTION("Event Driven Linux Device Driver which Records Position Of Mouse And Communicate Over UART/LED/ILI9225");
