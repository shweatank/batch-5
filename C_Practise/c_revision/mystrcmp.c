#include <stdio.h>
#include <string.h>

int mystrncmp(const char *s1,const char *s2,int n) {
	if(n > strlen(s2) && n > strlen(s1)) {
		printf("Invalid n bytes to compare\n");
		return -1;
	}
	else if(n > strlen(s1) && n < strlen(s2)) {
		n = strlen(s1);
	}
	else if(n > strlen(s2) && n < strlen(s1)) {
		n = strlen(s2);
	}
	int i;
	for(i =  0;i<n;i++) {
		if(s1[i] != s2[i]) {
			break;
		}
	}
	return s1[i] - s2[i];
}

int main() {
	printf("%d\n",mystrncmp("Hello","Hello",5));
}
