//show_bin_eq.c
#include <lpc21xx.h>
#include "types.h"
#include "delay.h"
#include "arduino_api_defines.h"
#include "arduino_api.h"
#define LED_AL_START 8 //p0.8
#define SW_AL 4 //p0.4
u32 seed = 0;
int main() {
	u32 num,i;
	//configure the pins p0.8 to p0.11 as output
	for(i = 8;i<=11;i++) {
		setPinMode(i,OUTPUT);
	}
	srand(seed++);
	while(1) {
		if(digitalRead(SW_AL)) {
			for(i = 8;i<=11;i++) {
				digitalWrite(i,1);
			}
		}
		else {
			delayMs(100);
			num = (rand() % 31) + 1;
			//num = 1;
			//for(i = 8;i<=11;i++) {
			//	digitalWrite(i,1);
			//}
			for(i = 8;i<=11;i++) {
				digitalWrite(i,!((num >> (i-8)) & 1));
			}
		}
	}
}
