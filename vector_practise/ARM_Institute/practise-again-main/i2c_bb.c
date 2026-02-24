#include <lpc21xx.h>
#include "lcd.c"

#define scl	2
#define sda	3

void i2c_start() {
	IOSET0 = 1 << scl | 1 << sda;
	IODIR0 |= 1 << scl | 1 << sda;
	IOCLR0 = 1 << sda;
}

void i2c_stop() {
	IOCLR0 = 1 << sda;
	IOSET0 = 1 << scl | 1 << sda;
}

void i2c_restart() {
	IOCLR0 = 1 << scl | 1 << sda;
	IOSET0 = 1 << scl | 1 << sda;
	IOCLR0 = 1 << sda;
}

void i2c_ack() {
	IODIR0 &= ~(1 << sda);	//make sda as input to read sda from slave
	IOSET0 = 1 << scl;
	while(((IOPIN0 >> sda) & 1));
	IOCLR0 = 1 << scl;
	IODIR0 |= 1 << sda;	// again made as output
}

void i2c_mack() {
	IODIR0 |= 1 << sda;	// making sda as op to write 0 onto the sda line
	IOCLR0 = 1 << scl;
	IOCLR0 = 1 << sda;
	IOSET0 = 1 << scl;
	IOCLR0 = 1 << scl;
}

void i2c_nack() {
	IODIR0 |= 1 << sda;	// making sda as op to write 0 onto the sda line
	IOCLR0 = 1 << scl;
	IOSET0 = 1 << sda;
	IOSET0 = 1 << scl;
	IOCLR0 = 1 << scl;
}

void i2c_write(char data) {
	int i;
	for(i = 7;i>=0;i--) {
		IOCLR0 = 1 << scl;	//first clear the scl
		IOPIN0 = (IOPIN0 & ~(1 << sda)) | ((data >> i) & 1);	// clear previous value and write data from msb
		IOSET0 = 1 << scl;	//the set the scl
	}
}

char i2c_read() {
	int i;
	char rchar;
	IODIR0 &= ~(1 << sda);	// making sda as inout to read data
	for(i = 7;i>=0;i--) {
		IOCLR0 = 1 << scl;
		delay_us(1);
		IOSET0 = 1 << scl;
		rchar = (rchar &~(1 << i)) | (((IOPIN0 >> sda) & 1) << i);
	}
	IODIR0 = 1 << sda;
	return rchar;
}
	
void i2c_eeprom_randomwrite(char saddr,char wbuffaddr,char data) {
	i2c_start();
	i2c_write(saddr<<1);
	i2c_ack();
	i2c_write(wbuffaddr);
	i2c_ack();
	i2c_write(data);
	i2c_ack();
	i2c_stop();
	delay_ms(10);
}

char i2c_eeprom_randomread(char saddr,char rbuffaddr) {
	char ch;
	i2c_start();
	i2c_write(saddr<<1);
	i2c_ack();
	i2c_write(rbuffaddr);
	i2c_ack();
	i2c_restart();
	i2c_write((saddr<<1) | 1);
	i2c_ack();
	ch = i2c_read();
	i2c_nack();
	i2c_stop();
	return ch;
}

char ch __attribute__ ((at(0x40000000)));

int main() {
//	init_lcd();
	IODIR0 |= 1 << 4 | 1 << 5;
	i2c_eeprom_randomwrite(0x50,0,'A');
	ch = i2c_eeprom_randomread(0x50,0);
	delay_ms(500);
	if(ch == 'A') {
		IOSET0 = 1 << 4;
	}
	else IOSET0 = 1 << 5;
//	char_lcd(ch);
	while(1);
}
