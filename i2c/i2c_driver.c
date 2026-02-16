#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/delay.h>

#define Base 0x7e205000
#define size 100
#define con 0x00
#define stat 0x04
#define DLEN 0x08
#define SLAVE 0x0c
#define DIV 0x14
#define FIFO 0x10
static void i2c_write(char);
static int i2c_read(void);
static void __iomem *i2c_base;

static int __init i2c_init(void){
        i2c_base=ioremap(Base,size);
        if(!i2c_base){
                pr_err("rpi_i2c:ioremap failed\n");
                return -ENOMEM;
        }
        writel(((0<<15)|(0<<9)|(0<<10)|(0<<8)),i2c_base+con); //dis i2c,ints on tx, rx
        writel((readl(i2c_base+DIV)&~(0x0000FFFF))|1500,i2c_base+DIV);
	writel((0<<4)|(0<<5),i2c_base+con);
	writel((1<<7)|(1<<15),i2c_base+con);
	writel((0x3c),i2c_base+SLAVE);
	pr_info("checking the code\n");
	char ch='A';
	i2c_write(ch);
	pr_info("wrote %c into fifo for slave\n",ch);
	ch='b';
	i2c_write(ch);
	pr_info("wrote %c to slave\n",ch);
	int x=i2c_read();
	pr_info("read %c from slave\n",x);
return 0;
}
static void i2c_write(char ch){
	int val=readl(i2c_base+con);
	val|=(1<<7);
	val&=~(0<<0);
	writel(val,i2c_base+con);
	writel(1,i2c_base+DLEN);
	writel((1<<0)|(1<<8)|(1<<1),i2c_base+stat);
	pr_info("wrote data %c into fifo\n",ch);
	while(!(readl(i2c_base+stat)&(1<<4)))
		cpu_relax();
	writel(ch,i2c_base+FIFO);
	while(!(readl(i2c_base+stat) & (1<<1)))
		cpu_relax();
	writel((1<<1),i2c_base+stat);
}
static int i2c_read(void){
	int val=readl(i2c_base+con);
	writel((1<<0)|(1<<7),i2c_base+con);
	writel(1,i2c_base+DLEN);
//	int val=read(i2c_base+stat);
	while(!(readl(i2c_base+stat)&((1<<5))))
		cpu_relax();
	val=readl(i2c_base+FIFO);
	while(!(readl(i2c_base+stat) & (1<<1)))
		cpu_relax();
	writel((1<<1),i2c_base+stat);
return val;
}
static void __exit i2c_exit(void){
	writel(0,i2c_base+con);
	writel((1<<1)|(1<<8)|(1<<9),i2c_base+stat);
	iounmap(i2c_base);
}

module_init(i2c_init);
module_exit(i2c_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("kavya");
MODULE_DESCRIPTION("i2c");
