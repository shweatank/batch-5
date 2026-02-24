#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void cmdAsCalculator(int argc,char *argv[]) {
	if(argc < 4) {
		printf("Invalid arguments\n");
		exit(0);
	}
	int val0 = 0,val2 = 0,i = 0;
	while(argv[2][i]) {
		val0 = val0*10+(argv[2][i] - '0');
		i++;
	}
	i = 0;
	while(argv[3][i]) {
		val2 = val2*10+(argv[3][i] - '0');
		i++;
	}
	switch(argv[1][0]) {
		case '+':
			printf("%d\n",val0+val2);
			break;
		case '-':
			printf("%d\n",val0-val2);
			break;
		case '*':
			printf("%d\n",val0*val2);
			break;
		case '/':
			printf("%d\n",val0/val2);
			break;
		case '%':
			printf("%d\n",val0%val2);
			break;
		case '=':
			if(argv[1][1])
				(val0 == val2) ? printf("TRUE\n") : printf("FALSE\n");
			break;
		default:
			printf("Invalid Arithematic Operator\n");
	}
}

int main(int argc,char *argv[]) {
	cmdAsCalculator(argc,argv);
}



