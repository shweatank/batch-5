#include <stdio.h>
int main() {
	int n;
	printf("Enter the number: ");
	scanf("%d",&n);
	int last = n % 10;
	int flag = 0;
	while(n) {
		int curr = n % 10;
		n /= 10;
		if(last >= curr) {
			last = curr;
			continue;
		}
		else {
			flag = 1;
			break;
		}
	}
	(flag) ? printf("Not-Ascending number\n") : printf("Ascending number\n");
}
