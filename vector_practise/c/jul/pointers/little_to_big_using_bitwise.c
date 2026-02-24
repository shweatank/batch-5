#include <stdio.h>
int swap(int);
int main() {
	int num = 0x12345678;
	printf("%x/n",swap(num));
}

int swap(int num) {
	char *p,ch;
	int res;
	p = (char *)&num;
	int bits0_7,bits8_15,bits16_23,bits24_31;
	bits0_7 = (num & 0x000000ff) >> 0;
	bits8_15 = (num & 0x0000ff00) >> 8;
	bits16_23 = (num & 0x00ff0000) >> 16;
	bits24_31 = (num & 0xff000000) >> 24;

	bits0_7 <<= 24;
	bits8_15 <<= 16;
	bits16_23 <<= 8;
	bits24_31 <<= 0;

	res = bits0_7 | bits8_15 | bits16_23 | bits24_31;
	return res;
}


