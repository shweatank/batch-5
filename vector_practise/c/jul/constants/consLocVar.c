#include <stdio.h>
int main() {
	const int var = 10;
	var++; // error bcoz it is constant and since it is local 
	       // varable , it is stored in stack
}
