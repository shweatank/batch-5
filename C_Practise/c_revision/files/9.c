#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char **argv) {
	if(argc != 3) {
		printf("Invalid arguments..!");
		return 0;
	}
	FILE *fp = fopen(argv[1],"r");
	if(!fp) {
		printf("Invalid file / file not found..!");
		return 0;
	}
	char str[100];
	int n = atoi(argv[2]),count1 = 0,count2 = 0;
	while(fgets(str,100,fp)) {
		count1++;
	}
	rewind(fp);
	while(fgets(str,100,fp)) {
		count2++;
		if(count2 > count1-n) {
			printf("%s",str);
		}
	}
}
		
		

