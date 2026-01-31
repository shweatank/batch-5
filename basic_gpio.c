#include <stdio.h>
#define PIN1   1

int main() {
	FILE *fp;
	fp = fopen("config.txt","r");
	if(!fp) {
		perror("fopen\n");
		return 0;
	}
	char buf[128];
	while(1) {
		if(fread(buf,sizeof(buf),1,fp))
			printf("%s\n",buf);
	}
}
