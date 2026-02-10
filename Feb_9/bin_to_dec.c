#include<stdio.h>
int main(){
	char str[]="10101001";
	int i,n=strlen(str),dec=0,base=1;
	for(i=n-1;i>=0;i--){
		if(str[i]=='1')
			dec+=base;
		base=base*2;
	}
	printf("%d",dec);
}
