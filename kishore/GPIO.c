#define IOSET0 *(volatile unsigned int*)0xE0028004
#define IOCLR0 *(volatile unsigned int*)0xE002800C
#define IODIR0 *(volatile unsigned int*)0xE0028018

void main(){
	int i;
	IODIR0=(1<<0);
	while(1){
		IOCLR0=(1<<0);
		for(i=0;i<1000000;i++);
		IOSET0=(1<<0);
	}
}
