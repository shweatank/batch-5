#include<linux/module.h>
#include<linux/init.h>
#include<linux/interrupt.h>
#include<linux/kernel.h>
#include<linux/io.h>

#define KBD_IRQ 1
#define DRIVER_NAME "keyboard_irq"
#define KBD_DATA_PORT 0x60

/* Simple US keyboard scan code map (Set 1) */
static const char *keymap[128] = {
    [0x01] = "ESC",
    [0x02] = "1",   [0x03] = "2",   [0x04] = "3",
    [0x05] = "4",   [0x06] = "5",   [0x07] = "6",
    [0x08] = "7",   [0x09] = "8",   [0x0A] = "9",
    [0x0B] = "0",
    [0x0C] = "-",   [0x0D] = "=",
    [0x0E] = "BACKSPACE",
    [0x0F] = "TAB",
    [0x10] = "Q",   [0x11] = "W",   [0x12] = "E",
    [0x13] = "R",   [0x14] = "T",   [0x15] = "Y",
    [0x16] = "U",   [0x17] = "I",   [0x18] = "O",
    [0x19] = "P",
    [0x1C] = "ENTER",
    [0x1D] = "CTRL",
    [0x1E] = "A",   [0x1F] = "S",   [0x20] = "D",
    [0x21] = "F",   [0x22] = "G",   [0x23] = "H",
    [0x24] = "J",   [0x25] = "K",   [0x26] = "L",
    [0x2C] = "Z",   [0x2D] = "X",   [0x2E] = "C",
    [0x2F] = "V",   [0x30] = "B",   [0x31] = "N",
    [0x32] = "M",
    [0x39] = "SPACE"
};

static irqreturn_t keyboard_isr(int irq,void*devid)
{
	unsigned char scancode;
	bool released;
	const char *key;
	scancode=inb(KBD_DATA_PORT);
	released=scancode&0x80;
	scancode&=0x7F;
	key=keymap[scancode];
	if(!key)
		key="UNKNOWN";
	if(released)
		pr_info("kbd irq:key released:%s-> scancode: 0x%02x\n",key,scancode);
	else
		pr_info("kbd_irq:key pressed :%s->scancode: 0x%02x\n",key,scancode);
	return IRQ_HANDLED;
}
static int __init keyboard_irq_init(void)
{
	int ret;
	ret=request_irq(KBD_IRQ,keyboard_isr,IRQF_SHARED,DRIVER_NAME,(void *)keyboard_isr);
	if(ret)
	{
		pr_err("%s:failed to rasie keyboard interrupt:%d\n",DRIVER_NAME,KBD_IRQ);
		return 0;
	}
	pr_info("%s: raised keyboard IRQ:%d successfully\n",DRIVER_NAME,KBD_IRQ);
	return 0;
}
static void __exit keyboard_irq_exit(void)
{
	free_irq(KBD_IRQ,(void *)keyboard_isr);
	pr_info("cleaning keyboard interrupt\n");
}
module_init(keyboard_irq_init);
module_exit(keyboard_irq_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("prasanna");
MODULE_DESCRIPTION("keyborad_irq");
