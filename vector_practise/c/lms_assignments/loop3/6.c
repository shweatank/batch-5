#include <stdio.h>
#include <stdbool.h>
bool descendingOrNot(int);
int main() {
	int num;
	scanf("%d",&num);
	(descendingOrNot(num)) ? printf("Descending\n") : printf("Not-Descending\n");
}
bool descendingOrNot(int num) {
	int last = num % 10;
	int c = 0;
	while(num) {
		int curr = num % 10;
		if(curr >= last) last = curr;
		else {
			c++;
			break;
		}
		num /= 10;
	}
	return (c == 0);
}
