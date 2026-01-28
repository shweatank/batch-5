#include<lpc21xx.h>

#define SCL 2
#define SDA 3

#define SI 3



void i2c_init(){
	PINSEL0|=1<<SCL|1<<SDA;

	I2SCLL=400;
	I2SCLH=400;

	I2CONSET=0x40;
}

void i2c_uninit(){
	
	I2CONCLR|=1<<6|1<<5|1<<2|1<<3;
}

void i2c_wait(){
	while(!(I2CONSET>>3&1));
}


void i2c_write(){
	I2CONSET=1<<5;	
	i2c_wait();
	I2CONCLR=1<<5|1<<3;

}


int main(){
	i2c_init();

	i2c_uninit();
}


