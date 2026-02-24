
int printf(const char*,...);
int f2();
void f1() {
	printf("Before the main block");
}
void main() {
	f1();
	printf("In the main block");
	f2();
}

int f2() {
	printf("After the main block");
	return 0;
}
