#include <stdio.h>
int fibonacci(int);
int main() {
	int num;
	scanf("%d",&num);
	for(int i = 0;i<=num;i++) {
		printf("%d ",fibonacci(i));
	}
}
int fibonacci(int num){
	if(num == 0) return 0;
	if(num == 1) return 1;
	return fibonacci(num-1)+fibonacci(num-2);
}
