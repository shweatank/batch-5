

#define IOPIN0 *((volatile unsigned int*)(0xE0028000))
#define IOSET0 *((volatile unsigned int*)(0xE0028004))
#define IODIR0 *((volatile unsigned int*)(0xE0028008))
#define IOCLR0 *((volatile unsigned int*)(0xE002800C))

#define LED 7

void delay_ms(int dly){
	dly*=12000;
	while(dly--);
}


int main(){

	IODIR0|=1<<7;
	while(1){
		IOSET0=1<<LED;
		delay_ms(10);
		IOCLR0=1<<LED;
		delay_ms(10);
	}
}
