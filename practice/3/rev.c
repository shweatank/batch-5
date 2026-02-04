//Reverse string using recursion................

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
char* reverse(char*,char*);

void main(){
	int len;
	char data[20];
	printf("Enter the string\n");
	scanf("%[^\n]",data);

	printf("Before=%s\n",data);
	for(len=0;data[len];len++);
	printf("After=%s\n",reverse(data,data+len-1));
}

char* reverse(char*first,char*last){

	static char temp;
	if(first<last){
		temp=*first;
		*first=*last;
		*last=temp;
		reverse(first+1,last-1);
	}
	return first;
}


