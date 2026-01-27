#define FOSC 12000000
#define CCLk (FOSC*5)
#define PCLK (CCLK/4)
#define ADCCLK 3000000
#define CLKDIV ((PCLK/ADCCLK)-1)

#define DONE 31
#define START 24
#define PDN 21
#define RES 6
#define CLKDIV_BITS 8

void adc_init(void)
{
	ADDCR|=(1<<PDN)|(CLKDIV<<CLKDIV_BITS);

}
void adc_read(int chno,int *dval)
{
	ADDCR=0xffffff00;
	ADDCR|=(1<<chno)|(1<<START);
	while(((ADDR>>DONE)&1)==0);
	ADDCR&=~(1<<START);
	*dval=((ADDR>>RES)&1023);


}

