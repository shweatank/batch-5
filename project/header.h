#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/gpio/consumer.h>
#include <linux/delay.h>
#include <linux/of.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include "font8x8_basic.h"
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/interrupt.h>
#include<linux/io.h>
#include<linux/gpio.h>
#include<linux/delay.h>
#include<linux/uaccess.h>
#include<linux/workqueue.h>
#include<linux/slab.h>
#include<linux/gfp.h>
#include<linux/init.h>

#define DRIVER_NAME "ili9225"
#define CLASS_NAME "ili"
#define DEVICE_NAME "ili9225_char"
#define LINE_HEIGHT 20

#define DHT_GPIO_4 4+512
#define GPIO_17 17+512
#define TIMER_PHY 0xFE003000
#define TIMER_CLO  0x04
#define TIMER_SIZE 0x1C
#define MSEC 1000
