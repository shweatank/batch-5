#include <stdio.h>
#include <unistd.h>
int countDigits(int max);
int main() {
	int min,max,width;
	printf("Enter the range: \n");
	printf("Enter min: ");
	scanf("%d",&min);
	printf("Enter max: ");
	scanf("%d",&max);
	width = countDigits(max);
	for(int i = min;i <= max; i++) {
		for(int j = 1;j <= 10;j++) {
			printf("%*d",width+2,i*j);
			fflush(stdout);
			usleep(30000);
		}
		printf("\n");
	}
//	printf("%d\n",countDigits(max));
}
int countDigits(int max) {
	int count = (max == 0) ? 1 : 0;
	while(max) {
		count++;
		max /= 10;
	}
	return count;
}

