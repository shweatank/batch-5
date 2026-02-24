//#include <stdio.h>
int printf(const char *format, ...);
void f2();
void f1() {
	printf("Before main function\n");
}
char main() {
	f1();
	printf("Main function\n");
	f2();
	return 'A';;
}
void f2() {
	printf("After main function\n");
}
