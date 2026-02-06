

#define spi_base 0xFE204000
#define CS 0x00
#define FIFO 0x04
#define CLK 0x08
#define DLEN 0x0c


static void __iomem *base;
void spi_init(void){
	base=ioremap(spi_base,0x100);

	writel((1<<3)|(1<<2)|(3<<4),CS+base);//transferring 8bits at a time,CPOL=1,CPHA=1,chip select=0

	writel(100,base+CLK);//setting clock

	writel(readl(base+CS)|(1<<5),base+CS);//enabling transmision

	char ch[10]="HELLO";
	for(int i=0;ch[i];i++){
		while(!(readl(base+CS)&(1<<18)));  //of FIFO is empty then allowing
		writel(ch[i],base+FIFO);
	}

	while(!(readl(base+CS)&((1<<16))));//allowing after all transmission is done


}


