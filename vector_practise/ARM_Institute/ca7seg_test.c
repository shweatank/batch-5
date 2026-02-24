#include "types.h"
#include "delay.h"
#include "seg.h"
#include <lpc21xx.h>

main() {
	u32 i;
	init_7seg();
	while(1) {
		for(i = 0;i<10;i++) {
			display_7seg(i);
			delay_s(1);
		}
	}
}
