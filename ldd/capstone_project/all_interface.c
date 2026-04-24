#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/interrupt.h>
#include<linux/io.h>
#include <linux/spi/spi.h>
#include <linux/gpio/consumer.h>
#include <linux/of.h>
#include <linux/device.h>
#include<linux/gpio.h>
#include<linux/delay.h>
#include<linux/workqueue.h>
#include<linux/types.h>
#include "char_header.h"

#define DHT_GPIO_4 4+512
#define GPIO_17 17+512

#define TIMER_PHY 0xFE003000
#define TIMER_CLO  0x04
#define TIMER_SIZE 0x1C
#define MSEC 1000
#define DRIVER_NAME "ili9225"
#define LINE_HEIGHT 20

#define LCD_WIDTH   220
#define LCD_HEIGHT  176

struct ili9225 {
    struct spi_device *spi;
    struct gpio_desc *rs;
    struct gpio_desc *reset;
};
static struct ili9225 *g_lcd,*lcd;

static int irq;
static struct workqueue_struct *my_wq;
static struct work_struct my_work;
static void __iomem *timer_base;

static void delay_us(unsigned int us)
{
u32 start,now;
start=readl(timer_base+TIMER_CLO);
while(1)
{
now=readl(timer_base+TIMER_CLO);
if((now-start)>=us)
	break;
}
}


static int ili9225_write16(struct ili9225 *lcd, u16 value)
{
    u8 buf[2];
    buf[0] = value >> 8;
    buf[1] = value & 0xFF;
    return spi_write(lcd->spi, buf, 2);
}



static int ili9225_write_reg(struct ili9225 *lcd, u16 reg, u16 data)
{
    gpiod_set_value(lcd->rs, 0);
    ili9225_write16(lcd, reg);

    gpiod_set_value(lcd->rs, 1);
    return ili9225_write16(lcd, data);
}






static void ili9225_reset(struct ili9225 *lcd)
{
    gpiod_set_value(lcd->reset, 1);
    msleep(5);
    gpiod_set_value(lcd->reset, 0);
    msleep(20);
    gpiod_set_value(lcd->reset, 1);
    msleep(50);
}





static void ili9225_init(struct ili9225 *lcd)
{
    ili9225_reset(lcd);

    ili9225_write_reg(lcd, 0x0001, 0x011C);
    ili9225_write_reg(lcd, 0x0002, 0x0100);

    // LANDSCAPE ENTRY MODE
    ili9225_write_reg(lcd, 0x0003, 0x1038);

    ili9225_write_reg(lcd, 0x0008, 0x0808);
    ili9225_write_reg(lcd, 0x000C, 0x0000);
    ili9225_write_reg(lcd, 0x000F, 0x0B01);

    ili9225_write_reg(lcd, 0x0010, 0x0A00);
    ili9225_write_reg(lcd, 0x0011, 0x1038);
    msleep(50);

    ili9225_write_reg(lcd, 0x0012, 0x1121);
    ili9225_write_reg(lcd, 0x0013, 0x0063);
    ili9225_write_reg(lcd, 0x0014, 0x5A00);
    msleep(50);

    ili9225_write_reg(lcd, 0x0007, 0x1017);
    msleep(20);
}





static void ili9225_fill(struct ili9225 *lcd, u16 color)
{
    int x, y;

    ili9225_write_reg(lcd, 0x0036, LCD_HEIGHT - 1);
    ili9225_write_reg(lcd, 0x0037, 0);
    ili9225_write_reg(lcd, 0x0038, LCD_WIDTH - 1);
    ili9225_write_reg(lcd, 0x0039, 0);

    ili9225_write_reg(lcd, 0x0020, 0);
    ili9225_write_reg(lcd, 0x0021, 0);

    gpiod_set_value(lcd->rs, 0);
    ili9225_write16(lcd, 0x0022);
    gpiod_set_value(lcd->rs, 1);

    for (y = 0; y < LCD_HEIGHT; y++)
        for (x = 0; x < LCD_WIDTH; x++)
            ili9225_write16(lcd, color);
}






static void drawPixel(int x, int y, uint16_t color)
{
    struct ili9225 *lcd = g_lcd;

    // New rotation for the circled corner
    int x_rotated = (LCD_WIDTH - 1) - x;
    int y_rotated = y;

    gpiod_set_value(lcd->rs, 0);
    ili9225_write16(lcd, 0x0020); 
    gpiod_set_value(lcd->rs, 1);
    ili9225_write16(lcd, y_rotated); // Use y_rotated for Gram Address Set

    gpiod_set_value(lcd->rs, 0);
    ili9225_write16(lcd, 0x0021);
    gpiod_set_value(lcd->rs, 1);
    ili9225_write16(lcd, x_rotated); // Use x_rotated for Gram Address Set

    gpiod_set_value(lcd->rs, 0);
    ili9225_write16(lcd, 0x0022);
    gpiod_set_value(lcd->rs, 1);
    ili9225_write16(lcd, color);
}
static void drawChar(int x, int y, unsigned char c, uint16_t color)
{
int scale=2;
    if (c < 32 || c > 126)
        return;

    const unsigned char *bitmap = font8x8[c - 32];

    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            if (bitmap[row] & (1 << (7 - col)))
            {
                for (int dy = 0; dy < scale; dy++)
                {
                    for (int dx = 0; dx < scale; dx++)
                    {
                        drawPixel(x + col * scale + dx, y + row * scale + dy, color);
                    }
                }
            }
        }
    }
}
static void drawString(int x, int y, const unsigned char *str, uint16_t color)
{
    int scale = 2;

    for (int i = 0; str[i]; i++)
    {
        drawChar(x, y, str[i], color);
        x += (8*scale)+2;   // adjust spacing
    }
}
static void sensor_work(struct work_struct *work)
{
 int i, j;
    u8 buf[5] = {0};
    char temp_str[25],hum_str[25]; 


   /* Start signal */
    gpio_direction_output(DHT_GPIO_4, 0);
   delay_us(18*MSEC);   
 
      gpio_set_value(DHT_GPIO_4, 1);
      delay_us(30);
    gpio_direction_input(DHT_GPIO_4);

    /* Response */
    if (gpio_get_value(DHT_GPIO_4))
        pr_info("GPIO_4 error in sensor work\n");

    while (!gpio_get_value(DHT_GPIO_4));
    while (gpio_get_value(DHT_GPIO_4));

    /* Read 40 bits */
    for (j = 0; j < 5; j++) {
        for (i = 0; i < 8; i++) {

            while (!gpio_get_value(DHT_GPIO_4));

            delay_us(30);
            if (gpio_get_value(DHT_GPIO_4))
                buf[j] |= (1 << (7 - i));

            while (gpio_get_value(DHT_GPIO_4));
        }
    }

    /* Checksum */
    if (((buf[0] + buf[1] + buf[2] + buf[3]) & 0xFF) != buf[4])
        pr_info("sensor data error\n");

pr_info("Temperature=%d.%d°C\n",buf[2],buf[3]);//tempearture -> buf[2]=integer part ,buf[3]=decimal part
pr_info("Humidity=%d.%d%%\n",buf[0],buf[1]);// humidity -> buf[0]=integer part buf[1]=fraction part 
  ili9225_fill(g_lcd, 0xFFFF);
snprintf(temp_str,sizeof(temp_str),"%d.%d%cC",buf[2],buf[3],126);
snprintf(hum_str,sizeof(hum_str),"%d.%d%%",buf[0],buf[1]);

drawString(5,10,"Temperature:",0x0000);
drawString(5,40,temp_str,0x0000);
drawString(5,70,"Humidity:",0x0000);
drawString(5,100,hum_str,0x0000);
enable_irq(irq);// enable irq
}
static irqreturn_t GPIO_isr(int irq, void *dev_id)
{
delay_us(50);
disable_irq_nosync(irq); //avoiding multiple triggers
	pr_info("GPIO interrrupt triggered\n");
    queue_work(my_wq, &my_work);
return IRQ_HANDLED;
}


/* ---------------------------------------------------- */
static int ili9225_probe(struct spi_device *spi)
{
    lcd = devm_kzalloc(&spi->dev, sizeof(*lcd), GFP_KERNEL);
    if (!lcd)
        return -ENOMEM;

    lcd->spi = spi;
    spi_set_drvdata(spi, lcd);

    lcd->rs = devm_gpiod_get(&spi->dev, "rs", GPIOD_OUT_LOW);
    lcd->reset = devm_gpiod_get(&spi->dev, "reset", GPIOD_OUT_HIGH);

    spi->mode = SPI_MODE_0;
    spi->bits_per_word = 8;
    spi_setup(spi);

    ili9225_init(lcd);
    ili9225_fill(lcd, 0xFFFF);

    g_lcd = lcd;
	if(!gpio_is_valid(GPIO_17))//  checking 17 present or not
		pr_err("Invalid GPIO 17\n");
	int rt=gpio_request(GPIO_17,"button"); // 17 as button
	if(rt)
		pr_info("gpio req failed\n");
	gpio_direction_input(GPIO_17); //set as input
	irq=gpio_to_irq(GPIO_17); //gpio number to irq  number
	int ret=request_irq(irq,GPIO_isr,IRQF_TRIGGER_RISING,"GPIO_ISR",NULL);
	if(ret){
		pr_err("request irq failed\n");
		return ret;
	}

        if(!gpio_is_valid(DHT_GPIO_4))
          pr_err("GPIO 4 invalid\n");

	/* GPIO request */
	ret = gpio_request(DHT_GPIO_4, "dht11_gpio");//set as sensor data pin
	if (ret) {
		pr_err("GPIO request failed\n");
		return ret;
	}
/* Create Workqueue */
    my_wq = create_singlethread_workqueue("dht_wq");
    if (!my_wq)
        return -ENOMEM;

    INIT_WORK(&my_work, sensor_work);

	gpio_direction_output(DHT_GPIO_4, 1); //set as output for sensor data pin

 timer_base=ioremap(TIMER_PHY,TIMER_SIZE);
    if (!timer_base) {
        pr_err("ioremap failed\n");
        return -ENOMEM;
    }
delay_us(1000);
	pr_info("module loaded\n");
	return 0;
}


static void ili9225_remove(struct spi_device *spi)
{
	disable_irq(irq);
	gpio_set_value(GPIO_17,0);
	gpio_free(GPIO_17);
	pr_info("GPIO_17 freed\n");

	gpio_free(DHT_GPIO_4);
	pr_info("GPIO_4 freed\n");
	free_irq(irq,NULL);
	
	flush_workqueue(my_wq);
	destroy_workqueue(my_wq);

if (timer_base){
        iounmap(timer_base);}

pr_info("removed\n");

}
static const struct of_device_id ili9225_dt_ids[] = {
    { .compatible = "ili9225_display" },
    { }
};
static struct spi_driver ili9225_driver = {
    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = ili9225_dt_ids,
    },
    .probe  = ili9225_probe,
    .remove = ili9225_remove,
};
MODULE_DEVICE_TABLE(of, ili9225_dt_ids);
module_spi_driver(ili9225_driver); 

MODULE_LICENSE("GPL");
MODULE_AUTHOR("team2");
MODULE_DESCRIPTION("GPIO + DHT11 + workqueue+timer interrupt + ioremap ");
