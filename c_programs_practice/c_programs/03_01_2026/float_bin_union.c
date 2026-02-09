#include<stdio.h>

union bin{
	int n;
	float f;
};


void print_bin(union bin *p){
	for(int i=31;i>=0;i--){
		printf("%d ",((p->n)>>i&1));
	}
}

int main(){
	union bin v;
	printf("enter float value:");
	scanf("%f",&v.f);
	print_bin(&v);
}

