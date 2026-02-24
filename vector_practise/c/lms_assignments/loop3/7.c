#include <stdio.h>
#include <stdbool.h>
bool ascending(int num);
bool descending(int num);
int main() {
        int num;
        scanf("%d",&num);
	(ascending(num) || descending(num)) ? printf("Sorted\n") : printf("Not-Sorted\n");
}

bool ascending(int num) {
	int last = num % 10;
	while(num) {
		if(num % 10 > last) return 0;
		last = num % 10;
		num /= 10;
	}
	return 1;
}

bool descending(int num) {
        int last = num % 10;
        while(num) {
                if(num % 10 < last) return 0;
                last = num % 10;
		num /= 10;
        }
        return 1;
}
