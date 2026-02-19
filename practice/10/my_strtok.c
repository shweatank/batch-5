#include<stdio.h>
#include<string.h>
void my_strtok(char*);
void main(){

	char *s="hello world hai bye ...";
	my_strtok(s);
}

void my_strtok(char*s){
	char p[100]={0};
	int i=0,j=0;
	while(1){
		if(*s==' ' || *s=='\0'){
			p[i]='\0';
			i=0;
			printf("%s\n",p);
		}
		else{
			p[i]=*s;
			i++;
		}
		s++;`
if(*s=='\0')
break;
	}
}
