#include<stdio.h>
int main()
{
	//int x = 0x12345678;
	int x=0x12ABCDEF;
    unsigned char *c = (char *)&x;
	void *v = &x;


	for(int i=0;i<sizeof(x);i++)
	{
		printf("%X\n",*(c+i));
		printf("%X\n ",*(unsigned char*)(v+i));
	}

}

/*/a.out
ffffffef FFFFFFEF ffffffcd FFFFFFCD ffffffab FFFFFFAB 12 12 */
/*har in C can be signed or unsigned depending on the platform.

On most systems, char is signed, which means its range is -128 to 127.

0xEF = 239 in decimal → out of signed char range, so it’s interpreted as -17.*/
