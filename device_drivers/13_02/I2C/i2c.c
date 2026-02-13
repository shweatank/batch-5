#define Base 0x7e205000
#define size 100 
#define con 0x00
#define stat 0x04
#define DLEN 0x08
#define SLAVE 0x0c
#define DIV 0x14
#define FIFO 0x10
static void __iomem *i2c_base;
static void __init i2c_init(void){
        i2c_base=ioremap(Base,size);
        if(!i2c_base){
                pr_err("rpi_i2c:ioremap failed\n");
                return -ENOMEM;
        }
        writel(((0<<15)|(0<<9)|(0<<10)|(0<<8)),Base+con); //dis i2c,ints on tx, rx
        writel((readl(i2c_base+DIV)&~(0000FFFF))|1500,i2c_base+DIV);
	writel((0<<4)|(0<<5),i2c_base+con);
	write((1<<7)|(1<<15),i2c_base+con);
}
static void i2c_write(char *ch){
	write((1<<7,i2c_base+con);
	int val=read(i2c_base+con);
	while(!(val&(1<<4)))
		cpu_relax();
	writel(ch,i2c_base+FIFO);

}
static void i2c_read(void){



}
static void __exit i2c_exit(void){


}


