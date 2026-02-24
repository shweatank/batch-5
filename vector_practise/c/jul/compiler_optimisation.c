#include <stdio.h>
void f1(int);
void f2(int,int);
void f3(int,int,int);
int main() {
	volatile int a = 10,b = 20,c = 30;
//	f1(a+1);
//	f1(++a);
//	f1(a++);
//	printf("a = %d\n",a);

//	f2(a++,--b);
//	printf("a = %d , b = %d\n",a,b);

	f3(a=b,a=0,++a);
}
void f1(int a) {
	printf("in f1...a = %d\n",a);
}
void f2(int x,int y) {
	++x,--y;
	printf("in f2... x = %d , y = %d\n",x,y);
}
void f3(int x,int y,int z) {
	printf("in f3... x = %d , y = %d , z = %d\n",x,y,z);
}
