#include <lpc21xx.h>
#include "delay.h"
#include "types.h"
#define SW_AL 4 //p0.4
#define LED_AL_START 8 //p0.8
#define LED_AL_END 15 //p0.15
unsigned int seed = 0;
int main() {
	unsigned int num;
	//configure the pins p0.8 to p0.15 as output
	IODIR0 |= (0xff << LED_AL_START);
	srand(seed++);
	while(1) {
		if((IOPIN0 >> SW_AL) & 1) {
			IOCLR0 = (0xff << LED_AL_START);
		}
		else {
			delayMs(100);
			//num = (rand() % 255) + 1;
			num = 120;
			//IOCLR0 = (0xff << LED_AL_START);
			//IOSET0 = (num << LED_AL_START);
			//IOPIN0 = ((IOPIN0 &~(0xff << LED_AL_START)) | (num << LED_AL_START)); 
			//         ( IOPIN0 is used only for reading and writing into pins is not allowed)
			IO0PIN = (IO0PIN & ~(0xFF << LED_AL_START)) | (num << LED_AL_START);
		}
	}
}