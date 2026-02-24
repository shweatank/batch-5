#include "types.h"
#include "delay.h"
#include "seg.h"
#include <lpc21xx.h>

main() {
	u32 i,dly;
	init_mux_7seg();
	while(1) {
		for(i = 0;i<100;i++) {
			for(dly = 150;dly>0;dly--) {				
				display_mux_7seg(i);
			}
			//delay_ms(1000);
		}
	}
}
