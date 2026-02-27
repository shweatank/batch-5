/*
   EVENT DRIVEN LINUX DEVICE DRIVER Integrated with:
   - Linux Input Subsystem (Mouse)
   - MMIO GPIO LED Control
   - MMIO UART Transmission
   - Character Device Interface
   - Workqueue for Deferred Display Drawing
   */

#include <linux/module.h>      // Module macros (module_init, MODULE_LICENSE)
#include <linux/kernel.h>      // Kernel logging (pr_info, pr_err)
#include <linux/init.h>        // __init and __exit
#include <linux/spi/spi.h>     // SPI subsystem
#include <linux/delay.h>       // msleep()
#include <linux/of.h>          // For Device Tree support
#include <linux/fs.h>          // Character device support
#include <linux/cdev.h>        // cdev structure
#include <linux/device.h>      // device_create()
#include <linux/uaccess.h>     // copy_from_user()
#include <linux/input.h>       // Input subsystem (mouse)
#include <linux/workqueue.h>   // Workqueue API
#include "font8x8_basic.h"     // 8x8 bitmap font for ili9225

/* ---------------- Driver Identification ---------------- */

#define DRIVER_NAME "ili9225"        // SPI driver name
#define CLASS_NAME  "ili"            // Class name in /sys/class
#define DEVICE_NAME "ili9225_char"   // Device name in /dev

/* ---------------- LCD Configuration ---------------- */

#define SCREEN_WIDTH 176			//width of display in pixels
#define SCREEN_HEIGHT 220			//heigth of display in pixels
#define MAX_STRING_LEN 128           //MAX Stored String Length

/* ---------------- MMIO GPIO (LED) ---------------- */

#define LED_PHYS_ADDR  0xFE200000   // GPIO base physical address
#define GPFSEL1 0x04                // Function select register offset
#define GPSET0  0x1c                // GPIO set register
#define GPCLR0  0x28                // GPIO clear register
#define LED_GPIO 17                 // GPIO pin 17 For LED

/* ---------------- MMIO UART ---------------- */

#define UART_PHYS_ADDR 0xFE201000   // UART base physical address
#define UART_FR 0x18                // Flag register
#define UART_DR 0x00                // Data register
#define UART_FR_TXFF (1<<5)         // TX FIFO Full flag
#define UART_CR 0x30                // Control register
#define UART_CR_UARTEN (1<<0)       // UART enable
#define UART_CR_TXE (1<<8)          // TX enable


static dev_t dev_num;                // Device number (major/minor)
static struct class *ili_class;      // Device class
static struct cdev ili_cdev;         // Character device structure
static struct ili9225 *g_lcd;        // Global LCD pointer

static int cursor_x = 0;             // Cursor X position
static int cursor_y = 0;             // Cursor Y position

static struct workqueue_struct *lcd_wq;  // Workqueue pointer
static struct work_struct lcd_work;      // Work structure
static char lcd_string[MAX_STRING_LEN];  // String to display on LCD

static void __iomem *gpio_base;      // Virtual GPIO base
static void __iomem *uart_base;      // Virtual UART base

/* ---------------- LCD Structure ---------------- */

struct ili9225 {
	struct spi_device *spi;      // SPI device pointer
	struct gpio_desc *dc;        // Data/Command GPIO
	struct gpio_desc *reset;     // Reset GPIO
};

/* ---------------- SPI Helpers ---------------- */

/* Send 16-bit value over SPI */
static int ili9225_write16(struct ili9225 *lcd, u16 value)
{
	u8 buf[2] = { value >> 8, value & 0xFF };  // Split into MSB/LSB
	return spi_write(lcd->spi, buf, 2);
}

/* Write register and data */
static int ili9225_write_reg(struct ili9225 *lcd, u16 reg, u16 data)
{
	gpiod_set_value(lcd->dc, 0);   // Command mode
	ili9225_write16(lcd, reg);

	gpiod_set_value(lcd->dc, 1);   // Data mode
	return ili9225_write16(lcd, data);
}

/* Hardware reset */
static void ili9225_reset(struct ili9225 *lcd)
{
	gpiod_set_value(lcd->reset, 1);  //pull reset gpio pin high
	msleep(5);			 //wait for 5ms
	gpiod_set_value(lcd->reset, 0);  //pull reset gpio pin low
	msleep(20);			 //wait for 20ms
	gpiod_set_value(lcd->reset, 1);  //pull reset gpio pin high
	msleep(50);			 //wait for 50ms
}

/* LCD initialization */
static void ili9225_init(struct ili9225 *lcd)
{
	ili9225_reset(lcd);     //reseting the lcd 

	/* LCD configuration sequence */
	ili9225_write_reg(lcd, 0x0001, 0x011C);// Set driver output control (scan direction & line config)
	ili9225_write_reg(lcd, 0x0002, 0x0100);// LCD driving control 
	ili9225_write_reg(lcd, 0x0003, 0x1030);// Set entry mode (GRAM write direction + color order)
	ili9225_write_reg(lcd, 0x0008, 0x0808);// Set blanking and front/back porch timing
	ili9225_write_reg(lcd, 0x000C, 0x0000);  //Selects internal clock
	ili9225_write_reg(lcd, 0x000F, 0x0B01); //Controls clock frequency
	ili9225_write_reg(lcd, 0x0010, 0x0A00); //Sets internal voltage
	ili9225_write_reg(lcd, 0x0011, 0x1038);//Enables internal circuits and starts charge pump
	msleep(50);//wait for 50ms to get voltage stability
	ili9225_write_reg(lcd, 0x0012, 0x1121); //Sets reference voltage for pixels brightness
	ili9225_write_reg(lcd, 0x0013, 0x0063); //Controls voltage switching
	ili9225_write_reg(lcd, 0x0014, 0x5A00);//Fine tuning
	msleep(50);//Delay for stabilizing voltage
	ili9225_write_reg(lcd, 0x0007, 0x1017);//Enables Internal logic, Display output
	msleep(20);//Delay to ensure stable start//Enables Internal logic, Display output
}

/* Fill entire screen with color */
static void ili9225_fill(struct ili9225 *lcd, u16 color)
{
	int x, y;

	/* Define full window */
	ili9225_write_reg(lcd, 0x0036, 175);  //set horizontal window end
	ili9225_write_reg(lcd, 0x0037, 0);//set horizontal window start
	ili9225_write_reg(lcd, 0x0038, 219);//set vertical window end
	ili9225_write_reg(lcd, 0x0039, 0);//set vertical window start
	ili9225_write_reg(lcd, 0x0020, 0);//horizontal start address
	ili9225_write_reg(lcd, 0x0021, 0);//vertical start address

	gpiod_set_value(lcd->dc, 0);//command mode
	ili9225_write16(lcd, 0x0022);//GRAM data register
	gpiod_set_value(lcd->dc, 1);//Data mode

	for (y = 0; y < SCREEN_HEIGHT; y++)   	       /*Filling LCD Screen With Colour*/
		for (x = 0; x < SCREEN_WIDTH; x++)
			ili9225_write16(lcd, color);//send color for each pixel
}

/* Draw single pixel */
static void drawPixel(int x, int y, uint16_t color)
{
	struct ili9225 *lcd = g_lcd;

	gpiod_set_value(lcd->dc, 0);//command mode
	ili9225_write16(lcd, 0x0020);//Register for X address
	gpiod_set_value(lcd->dc, 1);//Data mode
	ili9225_write16(lcd, x);//Write x Coordinate

	gpiod_set_value(lcd->dc, 0);//command mode
	ili9225_write16(lcd, 0x0021);//register for Y address
	gpiod_set_value(lcd->dc, 1);//Data mode
	ili9225_write16(lcd, y);//Write Y coordinate

	gpiod_set_value(lcd->dc, 0);//command mode
	ili9225_write16(lcd, 0x0022);//GRAM data register for pixel color
	gpiod_set_value(lcd->dc, 1);//Data mode
	ili9225_write16(lcd, color);//Write the 16-bit color value
}

/* Draw character (scaled 2x) */
static void drawChar(int x, int y, char c, uint16_t color)
{
	if(c < 32 || c > 127) return;

	const uint8_t *bitmap = font8x8[c - 32];
	/* Loop through each row and each column */
	for(int row = 0; row < 8; row++)
		for(int col = 0; col < 8; col++)
			/* Check if the pixel bit is ON in the bitmap*/
			if(bitmap[row] & (1 << (7 - col))) {
				drawPixel(x + col*2, y + row*2, color);//top left
				drawPixel(x + col*2+1, y + row*2, color);//top right
				drawPixel(x + col*2, y + row*2+1, color);//bottom left
				drawPixel(x + col*2+1, y + row*2+1, color);//bottom right
			}
}

/* Draw string */
static void drawString(int *x, int *y, const char *str, uint16_t color)
{
	int start_x = *x;//saving the original x position for line resets
	/*Loop through the end of the string*/
	for(int i = 0; str[i]; i++)
	{
		/*if new line is found move to next line*/
		if(str[i] == '\n') {
			*x = start_x;//reset x to start of line
			*y += 16;//move y down by one character height
			if(*y > SCREEN_HEIGHT-16) *y = 0;
			continue;//skip drawing this character
		}

		drawChar(*x, *y, str[i], color);//drawing the current character at current cursor
		*x += 16;//position for next character

		if(*x > SCREEN_WIDTH - 16) {
			*x = start_x;//reset to start of line
			*y += 16;//next line down
			if(*y > SCREEN_HEIGHT-16) *y = 0;//wrap to top
		}
	}
}

/* Workqueue function */
static void lcd_work_func(struct work_struct *work)
{
	ili9225_fill(g_lcd, 0xFFFF);      // White background
	cursor_x = cursor_y = 0;//make cursor x and y 0
	drawString(&cursor_x, &cursor_y, lcd_string, 0x0000);  // Black text
}

/* ---------------- GPIO LED ---------------- */

static void gpio17_set_output(void)
{
	u32 val = ioread32(gpio_base + GPFSEL1);
	val &= ~(0x7 << 21);//clearing the 3 bits in GPFSEL1
	val |= (0x1 << 21);// seting the 1 bit as high
	iowrite32(val, gpio_base + GPFSEL1);//making gpio 17 as output
}

static void led_on(void)
{
	iowrite32(1 << LED_GPIO, gpio_base + GPSET0); //GPSET0 is high for led ON 
}

static void led_off(void)
{
	iowrite32(1 << LED_GPIO, gpio_base + GPCLR0);  //GPCLR0 is high for led OFF 
}

/* ---------------- UART ---------------- */

static void uart_send_char(char c)
{
	if(!uart_base) return;

	while(ioread32(uart_base + UART_FR) & UART_FR_TXFF) //waiting until TXFF is empty
		cpu_relax();

	iowrite32(c, uart_base + UART_DR);//writing the data into the data register 
}

static void uart_send_string(const char *str)
{
	while(*str)
		uart_send_char(*str++); //sending character by character to uart_send_char
}

/* ---------------- Character Device ---------------- */

static char stored_string[MAX_STRING_LEN]={"MOUSE_EVENT"};  //storing the string from the user

static ssize_t ili_write(struct file *file,
		const char __user *buf,
		size_t len,
		loff_t *off)
{
	if(len > MAX_STRING_LEN-1)
		len = MAX_STRING_LEN-1;

	if(copy_from_user(stored_string, buf, len))   //copying from the user into the buffer 
		return -EFAULT;

	stored_string[len] = '\0';

	pr_info("Stored string = %s\n", stored_string);
	return len;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.write = ili_write,
};

/* ---------------- Mouse Input ---------------- */

static int x_pos = SCREEN_WIDTH/2;
static int y_pos = SCREEN_HEIGHT/2;
static struct input_handle *mouse_handle;

//flags for checking the conditions
static bool mouse_connected = false;
static bool quadrant_active = false;

static void mouse_event(struct input_handle *handle,
		unsigned int type,
		unsigned int code,
		int value)
{
	if(!mouse_connected)
		return;

	if(type == EV_REL){
		if(code == REL_X){
			x_pos += value;//updating the x_pos 
			if(x_pos<0)
				x_pos=0;//update with x_pos with start
			if(x_pos>SCREEN_WIDTH)
				x_pos=SCREEN_WIDTH;//updating x_pos without going beyond the screen_width
		}
		if(code == REL_Y){
			y_pos += value; //updating the y_pos
			if(y_pos<0)
				y_pos=0; //update with y_pos with start
			if(y_pos>SCREEN_HEIGHT) 
				y_pos=SCREEN_HEIGHT;//updating y_pos without going beyond the screen_height
		}
	}

	/* Checking for is Quadrant is activated by user or not*/
	if(type == EV_KEY && code == BTN_RIGHT && value == 1){
		quadrant_active = !quadrant_active;
		pr_info("Quadrant logic %s\n", quadrant_active ? "activated" : "deactivated");
		return;
	}

	if(!quadrant_active)
		return;


	/* Left click used for the quadrant actions */
	if(type == EV_KEY && code == BTN_LEFT && value == 1){

		if(x_pos < SCREEN_WIDTH/2 && y_pos < SCREEN_HEIGHT/2) //checking for the top left quadrant condition
		{
			pr_info("TOP LEFT -> send string via uart: %s\n",stored_string);
			uart_send_string(stored_string);//sending stored string over uart
			uart_send_string("\n");
		}
		else if(x_pos > SCREEN_WIDTH/2 && y_pos > SCREEN_HEIGHT/2) //checking for the bottom right quadrant condition
		{
			pr_info("BOTTOM RIGHT -> Send String to LCD\n");
			strncpy(lcd_string, stored_string, MAX_STRING_LEN-1); //copying the stored string into the lcd_string
			queue_work(lcd_wq, &lcd_work);//work is given through workqueue in process context
		}
		else if(x_pos > SCREEN_WIDTH/2 && y_pos < SCREEN_HEIGHT/2)//checking for the top right quadrant condition
		{
			pr_info("TOP RIGHT -> LED ON\n");
			led_on();//led is ON
		}
		else{
			pr_info("BOTTOM LEFT -> LED OFF\n");
			led_off();//led is OFF
		}
	}
}


static int mouse_connect(struct input_handler *handler,
		struct input_dev *dev,
		const struct input_device_id *id)
{
	mouse_handle = kzalloc(sizeof(*mouse_handle), GFP_KERNEL);//allocating memory for the mouse_handle 
	if(!mouse_handle)
		return -ENOMEM;
	/*set up the handle: device handler and name*/
	mouse_handle->dev = dev;
	mouse_handle->handler = handler;
	mouse_handle->name = "mouse_ili_handle";

	if(input_register_handle(mouse_handle)){
		kfree(mouse_handle);
		return -EINVAL;
	}
	/*opening the input device to start the handler*/
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
	input_close_device(handle);//closing the input device
	input_unregister_handle(handle);//unregistering the input handle
	kfree(handle);
	mouse_connected = false;
	quadrant_active = false; // reset toggle on disconnect
}

static const struct input_device_id mouse_ids[] = {
	{ .flags = INPUT_DEVICE_ID_MATCH_EVBIT, 
		.evbit = { BIT_MASK(EV_KEY) | BIT_MASK(EV_REL) },
	},
	{}
};

static struct input_handler mouse_handler = {
	.event = mouse_event,
	.connect = mouse_connect,
	.disconnect = mouse_disconnect,
	.name = "mouse_ili_handler",
	.id_table = mouse_ids,
};

/* ---------------- SPI Driver ---------------- */

static int ili9225_probe(struct spi_device *spi)
{
	struct ili9225 *lcd;

	lcd = devm_kzalloc(&spi->dev, sizeof(*lcd), GFP_KERNEL);
	if(!lcd)
		return -ENOMEM;

	lcd->spi = spi;//store spi device
	spi_set_drvdata(spi, lcd);//bind data to this device

	lcd->dc = devm_gpiod_get(&spi->dev, "dc", GPIOD_OUT_LOW);//get data from the DTS and set initial low
	if(IS_ERR(lcd->dc))
		return PTR_ERR(lcd->dc);//return err if pin not available
	lcd->reset = devm_gpiod_get(&spi->dev, "reset", GPIOD_OUT_HIGH);//get data from DTS and set initially high
	if(IS_ERR(lcd->reset))
		return PTR_ERR(lcd->reset);//return err if pin not available

	//Creating Virtual Address using IOREMAP for LED
	gpio_base = ioremap(LED_PHYS_ADDR, 0xB4);
	if(!gpio_base)
	{
		pr_err("Failed ioremap GPIO\n");
		return -ENOMEM;
	}
	gpio17_set_output();//configure LED as output
	led_off();//turn led off
	/*Map 256 Bytes For UART*/
	uart_base = ioremap(UART_PHYS_ADDR, 0x1000);
	if(!uart_base)
	{
		iounmap(gpio_base);
		return -ENOMEM;
	}
	iowrite32(UART_CR_UARTEN | UART_CR_TXE, uart_base + UART_CR);

	spi->mode = SPI_MODE_0; //spi mode is 0
	spi->bits_per_word = 8;//8 bit transfers
	spi_setup(spi);//apply configuration

	ili9225_init(lcd);//send init register sequence
	ili9225_fill(lcd, 0xFFFF);//clear screen with white
	g_lcd = lcd;//save a global pointer for later use

	/* Character Device Allocation*/
	int ret= alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
	if(ret)
		return ret;
	cdev_init(&ili_cdev, &fops);//initialize cdev structure
	cdev_add(&ili_cdev, dev_num, 1);//add cdev  to the system

	ili_class = class_create(CLASS_NAME);//creating the class in /dev
	if(IS_ERR(ili_class))
	{
		unregister_chrdev_region(dev_num,1);
		return PTR_ERR(ili_class);
	}
	device_create(ili_class, NULL, dev_num, NULL, DEVICE_NAME);

	/*Creating WorkQueue for LCD */
	lcd_wq = create_singlethread_workqueue("lcd_wq");
	INIT_WORK(&lcd_work, lcd_work_func);//initialize the work structure

	/*Registering the Mouse Handler */
	ret = input_register_handler(&mouse_handler);
	if(ret)
		pr_warn("Failed Input_register_handler\n");
	pr_info("Event Device Driver is Loaded\n");

	return 0;
}

static void ili9225_remove(struct spi_device *spi)
{
	ili9225_fill(g_lcd,0xFFFF);  //Filling the LCD With White
	flush_workqueue(lcd_wq);//flushing the workqueue
	destroy_workqueue(lcd_wq);//destroying the workqueue

	input_unregister_handler(&mouse_handler);//unregistering the input handler

	device_destroy(ili_class, dev_num);//destroy the device
	class_destroy(ili_class);
	cdev_del(&ili_cdev);
	unregister_chrdev_region(dev_num, 1);//unregister the character device region

	if(gpio_base)
		iounmap(gpio_base);
	if(uart_base)
		iounmap(uart_base);
	pr_info("Event Device Driver is Unloaded\n");
}

/* Device Tree */
static const struct of_device_id ili9225_dt_ids[]={
	{.compatible = "ilitek,ili9225"},
	{}
};
MODULE_DEVICE_TABLE(of,ili9225_dt_ids);

/* ---------------- Module ---------------- */

static struct spi_driver ili9225_driver = {
	.driver = {
		.name = DRIVER_NAME,
		.of_match_table = ili9225_dt_ids,
	},
	.probe  = ili9225_probe,
	.remove = ili9225_remove,
};

static int __init mouse_spi_init(void)
{
	pr_info("Registering SPI Driver and Mouse Handler\n");
	return spi_register_driver(&ili9225_driver);
}
static void __exit mouse_spi_exit(void)
{
	spi_unregister_driver(&ili9225_driver);
	pr_info("SPI driver unregistered\n");
}
module_init(mouse_spi_init);
module_exit(mouse_spi_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TEAM 4 PROJECT");
MODULE_DESCRIPTION("Event Driven Linux Device Driver Which Records The Position Of Mouse And Communicate Over Various Interfaces");
