
#define GPIO_BASE 0xFE200000
volatile unsigned int*gpio=(volatile unsigned int*)GPIO_BASE;

void delay(int n) {
	for (volatile int i=0;i<n;i++);
}

int main(){

	gpio[1] = 0x00200000;

    while (1) {
        gpio[7]  = 0x00020000; 
        delay(1000000);
        gpio[10] = 0x00020000; 
        delay(1000000);
    }

    return 0;
}
