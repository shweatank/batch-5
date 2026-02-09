#include<stdio.h>


int check(int p){
	int i=0,cnt=0;
	while(i<32){
		if((p>>i&1))
			cnt++;
		i++;
	}
	if(cnt==2)
		return 1;
	return 0;

}

int main(){
	int n,cnt=0;
	int i;
	printf("enter n:");
	scanf("%d",&n);
		
	for(i=3;i;){
		if(check(i)){
			cnt++;
			if(cnt==n)
				break;
			else
				i++;
		}
		else{
			i++;
		}
	}
	printf("%d\n",i);

}
