#include <lpc21xx.h>
#include <stdlib.h>
#include "lcd.c"

#define fosc					12000000
#define cclk					fosc*5
#define pclk					(cclk/4)
#define i2c_speed			100000	//100KHz
#define i2c_bitrate		((pclk/i2c_speed)/2)

#define i2en	6
#define sta		5
#define	sto		4
#define si		3
#define aa		2

#define i2conset	I2CONSET
#define	i2conclr	I2CONCLR
#define i2dat			I2DAT
#define i2stat		I2STAT
#define	i2scll		I2SCLL
#define i2sclh		I2SCLH

void init_i2c() {
	PINSEL0 |= 0x50;	// to configure pin 2 and 3 as scl and sda 
	i2scll = i2c_bitrate;
	i2sclh = i2c_bitrate;
	i2conset = 1 << i2en;
}

void i2c_start() {
	i2conset = 1 << sta;
	i2conclr = 1 << si;
	while(((i2conset >> si) &1) == 0);
	i2conclr = 1 << sta;
}

void i2c_stop() {
	i2conset = 1 << sto;
	i2conclr = 1 << si;
}

void i2c_restart() {
	i2conset = 1 << sta;
	i2conclr = 1 << si;
	while(((i2conset >> si) & 1) == 0);
	i2conclr = 1 << sta;
}

char i2c_mack() {
	i2conset = 1 << aa;
	i2conclr = 1 << si;
	while(((i2conset >> si) & 1) == 0);
	return i2dat;
}

char i2c_nack() {
	i2conclr = 1 << aa;
	i2conclr = 1 << si;
	while(((i2conset >> si) & 1) == 0);
	return i2dat;
}

//int i2c_slaw_ack() {
//	return (i2stat == 0x18);
//}

//int i2c_slar_ack() {
//	return (i2stat == 0x40);
//}

//int i2c_data_ack() {
//	return (i2stat == 0x28);
//}

void i2c_write(char data) {
	i2dat = data;
	i2conclr = 1 << si;
	while(((i2conset >> si) & 1) == 0);
}

char i2c_read() {
	return i2c_mack();
}

char i2c_last_read() {
	return i2c_nack();
}

void i2c_eeprom_bytewrite(char saddr,char wbuffaddr,char data) {
	i2c_start();
	i2c_write(saddr << 1);
//	if(i2c_slaw_ack() == 0) exit(0);
	i2c_write(wbuffaddr);
//	if(i2c_data_ack() == 0) exit(0);
	i2c_write(data);
//	if(i2c_data_ack() == 0) exit(0);
	i2c_stop();
	delay_ms(10);
}

char i2c_eeprom_byteread(char saddr,char rbuffaddr) {
	char data;
	i2c_start();
	i2c_write(saddr << 1);
//	if(i2c_slaw_ack() == 0) exit(0);
	i2c_write(rbuffaddr);
//	if(i2c_data_ack() == 0) exit(0);
	i2c_restart();
	i2c_write((saddr << 1) | 1);
//	if(i2c_slar_ack() == 0) exit(0);
	data = i2c_last_read();
	i2c_stop();
	return data;
}
