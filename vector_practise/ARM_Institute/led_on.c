#include <lpc21xx.h>
#include "types.h"
#include "delay.h"
main() {
	IODIR0 |= 1 << 4;
	while(1) {
		IOSET0 = 1 << 4;
		delayMs(500);
		IOCLR0 = 1 << 4;
		delayMs(500);
	}
}
