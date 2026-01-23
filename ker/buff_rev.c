#include<stdio.h>
int main(){
	char buff[40]="10,20,+";
	int t=30,i=0;
	while(t){
		buff[i++]=t%10+'0';
		t/=10;
	}
	buff[i]='\0';
	char *p,*q,temp;
        p=buff;
        q=buff+strlen(buff)-1;
        while(p<q){
                temp=*p;
                *p=*q;
                *q=temp;
                p++,q--;
        }
	printf("%s",buff);
}
