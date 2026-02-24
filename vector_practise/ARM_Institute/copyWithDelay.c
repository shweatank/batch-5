#include "delay.h"
const char str[] = "Vector India PVT LTD";
char dest[100];
int i,count;
int main() {
	i = 0;count = 0;
	while(str[i]) {
		if(str[i] == ' ') count++;
		if(count == 0) {
			//delayS(1);
			dest[i] = str[i];
		}
		if(count == 1) {
			//delayMs(500);
			dest[i] = str[i];
		}
		if(count == 2) {
			//delayMs(100);
			dest[i] = str[i];
		}
		if(count == 3) {
			delayUs(100);
			dest[i] = str[i];
		}
		i++;
	}
	dest[i] = '\0';
	while(1);	
}
