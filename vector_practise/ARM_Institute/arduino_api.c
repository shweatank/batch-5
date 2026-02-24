//arduino_api_.c
#include <lpc21xx.h>
#include "types.h"
#include "arduino_api_defines.h"
#include "arduino_api.h"
void setPinMode(u32 pinNo,u32 pinDir) {
	if(pinNo <= 31) {
		(pinDir == OUTPUT) ? IODIR0 |= (1 << pinNo) : (IODIR0 &= ~(1 << pinNo));
	}
	else if(pinNo >= 32 && pinNo <= 46) {
		(pinDir == OUTPUT) ? IODIR1 |= (1 << (pinNo-16)) : (IODIR1 &= ~(1 << (pinNo-16)));
	}
}

void digitalWrite(u32 pinNo,u32 bit) {
	if(pinNo <= 31) {
		IO0PIN |= ((IO0PIN &~(1 << pinNo)) | (bit << pinNo));
	}
	else if(pinNo >= 32 && pinNo <= 46) {
		IO1PIN |= ((IO1PIN &~(1 << (pinNo-16))) | (bit << (pinNo-16)));
	}
}

u32 digitalRead(u32 pinNo) {
	u32 status;
	if(pinNo <= 31) {
		status = ((IO0PIN >> pinNo) &1);
	}
	else if(pinNo >= 32 && pinNo <=46) {
		status = ((IO1PIN >> pinNo) &1);
	}
	return status;
}
