#include <lpc21xx.h>

#define	lcd_data	8
#define lcd_rs		16
#define lcd_en		17
#define lcd_rw		18

#define writebyte(word,pinno,data)		word = ((word & ~(0xFF << pinno)) | (data << pinno))

#define clear_lcd	0x01

#define	disp_off_cur_off	0x08
#define	disp_on_cur_on		0x0E
#define disp_on_cur_off 	0x0C
#define disp_off_cur_on		0x0A
#define disp_on_cur_blink	0x0F

#define shift_cur_right		0x06
#define shift_cur_left		0x07

#define mode_8bit_1line		0x30
#define	mode_8bit_2line		0x38

#define goto_line1_pos0		0x80
#define goto_line2_pos0		0xC0

#define	goto_cgram_start	0x40

void delay_ms(int num) {
	num *= 12000;
	while(num--);
}

void delay_us(int num) {
	num *= 12;
	while(num--);
}

void writelcd(char data) {
	IOCLR0 = 1 << lcd_rw;
	writebyte(IOPIN0,lcd_data,data);
	IOSET0 = 1 << lcd_en;
	delay_us(1);
	IOCLR0 = 1 << lcd_en;
	delay_ms(1);
}

void cmdlcd(char cmd) {
	IOCLR0 = 1 << lcd_rs;
	writelcd(cmd);
}

void init_lcd() {
	IODIR0 |= (0xFF << lcd_data) | (1 << lcd_rw) | (1 << lcd_rs) | (1 << lcd_en);
	delay_ms(15);
	cmdlcd(mode_8bit_1line);
	delay_ms(5);
	cmdlcd(mode_8bit_1line);
	delay_ms(1);
	cmdlcd(mode_8bit_1line);
	cmdlcd(mode_8bit_2line);
	cmdlcd(disp_on_cur_on);
	cmdlcd(clear_lcd);
	cmdlcd(shift_cur_right);
}

void char_lcd(char data) {
	IOSET0 = 1 << lcd_rs;
	writelcd(data);
}

void int_lcd(int num) {
	int res = 0;
	while(num) {
		res = (res*10) + num % 10;
		num /= 10;
	}
	while(res) {
		char_lcd(res % 10 + '0');
		res /= 10;
	}
}

void build_cgram(char *str,int bytes) {
	int i;
	cmdlcd(goto_cgram_start);
	IOSET0 = 1 << lcd_rs;
	for(i = 0;i<bytes;i++) {
		writelcd(str[i]);
	}
	cmdlcd(0x80);
}

//int main() {
//	char str[8] = {0x04,0x0e,0x15,0x04,0x04,0x04,0x04,0x04};
//	init_lcd();
//	build_cgram(str,8);
//	while(1) {
//		cmdlcd(goto_line1_pos0);
//		//char_lcd('8');

//		char_lcd(0);
//	}
//}
