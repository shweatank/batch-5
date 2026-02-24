#include <lpc21xx.h>
#include "lcd.c"

#define rows_4	16
#define cols_4	20

char kpmlut[4][4] = {
		{'1','2','3','/'},
		{'4','5','6','*'},
		{'7','8','9','+'},
		{'A','0','C','-'}
	};

void init_kpm() {
	IODIR1 |= 0xf << rows_4; // rows made as gpio op and zero
	//cols made as gpio ip and high (default)
}

int colscan() {
	return (((IOPIN1 >> cols_4) & 0x0f) < 15) ? 0 : 1;
}

int rowcheck() {
	int r;
	for(r = 0;r <= 3;r++) {
		IOSET1 = (0xF << rows_4);              // make all rows high
    IOCLR1 = (1 << (rows_4 + r));					 // make 1 row ground at once
		if(colscan() == 0) break;
	}
	IOCLR1 = 0xf << rows_4;
	return r;
}

int colcheck() {
	int c;
	for(c = 0;c <= 3;c++) {
		if(((IOPIN1 >> (cols_4+c)) & 1) == 0) break;
	}
	return c;
}

char keyscan() {
	int r,c;
	while(colscan());
	r = rowcheck();
	c = colcheck();
	return kpmlut[r][c];
}

int readnum() {
	int res = 0;
	char key;
	while(1) {
		key = keyscan();
		if(key >= '0' && key <= '9') {
			res = (res*10) + (key - '0');
			while(colscan() == 0);
		}
		else {
			while(colscan() == 0);
			break;
		}
	}
	return res;
}

//int main() {
//	int num;
//	init_kpm();
//	init_lcd();
//	while(1) {
//		num = readnum();
//		int_lcd(num);
//		delay_ms(200);
//	}
//}
