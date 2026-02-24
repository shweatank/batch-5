#include<stdio.h>
#include<string.h>
#include<stdlib.h>

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

int isprime(int n){
	if(n==0||n==1)
		return 0;
	for(int i=2;i<n;i++){
		if(n%i==0)
			return 0;
	}
	return 1;
}

int set_cnt(char ch){
	int cnt=0,i;
	for(i=0;i<8;i++){
		if(ch>>i&1)
			cnt++;
	}
	return cnt;
}

int main(){
	char *p=NULL;
	int *arr=NULL,c=0;
	int cnt=0;
	p=getstring();
	printf("%s\n",p);
	
	for(int i=0;p[i];i++){
		cnt=set_cnt(p[i]);
		printf("%c %d\n",p[i],cnt);
		if(isprime(i)&&isprime(cnt)){
			arr=realloc(arr,(c+1)*sizeof(int));
			arr[c++]=i;
			//memmove(p+i,p+i+1,strlen(p+i+1)+1);
			//i--;
		}
	}

	for(int i=0;i<c;i++){
		//printf("%d\n",arr[i]);
		memmove(p+arr[i],p+arr[i]+1,strlen(p+arr[i]+1)+1);
		for(int j=i+1;j<c;j++){
			arr[j]--;		
		}
	}

	printf("%s\n",p);
}


