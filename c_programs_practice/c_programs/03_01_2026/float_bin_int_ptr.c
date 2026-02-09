#include<stdio.h>

int main(){
	int *p;
	float f;
	printf("enter float:");
	scanf("%f",&f);
	p=(int*)&f;
	for(int i=31;i>=0;i--){
		printf("%d ",(*p)>>i&1);
	}

}

