#include <stdio.h>
int checkBitStatus(int ,int);
int toggleBits(int,int,int);
int main() {
	int num;
	scanf("%d",&num);
	for(int i = 0,j=31;j>i;i++,j--) {
		if(checkBitStatus(num,i) != checkBitStatus(num,j)) {
			num = toggleBits(num,i,j);
		}
		printf("%d",checkBitStatus(num,j));
		printf("%d",checkBitStatus(num,i));
	}
	printf("\n");
	printf("%d\n",num);
}
int checkBitStatus(int num,int bitposition) {
	return (num >> bitposition)&1;
}
int toggleBits(int num,int i,int j) {
	num ^= (1 << i);
	return num ^= (1 << j);
}

