#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char *getstring(){
	char *p=NULL;
	int i=0;
	do{
		p=realloc(p,i+1);
		p[i]=getchar();
	}while(p[i++]!='\n');
	p[--i]='\0';
	return p;
}

void rev(char *s,int i,int j){
	if(i>j){
		return;
	}
	char t=s[i];
	s[i]=s[j];
	s[j]=t;
	i++;j--;
	rev(s,i,j);
}

int main(){
	char *s=NULL;
	printf("enter string:");
	s=getstring();
	printf("Before Reverse:%s\n",s);
	rev(s,0,strlen(s)-1);
	printf("After Reverse:%s\n",s);
}




