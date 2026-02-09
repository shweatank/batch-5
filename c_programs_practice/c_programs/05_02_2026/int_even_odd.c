#include<stdio.h>


//123456
//o=531
//e=642

int split(int n,char ch){
	int s=0;
	switch(ch){
		case 'o':
			while(n){
				if((n%10)%2){
					s=s*10+(n%10);
				}
				n/=10;
			}
			break;
		case 'e':
			while(n){
				if((n%10)%2==0){
					s=s*10+(n%10);
				}
				n/=10;
			}	
			break;
	}
	return s;
}

int merge(int o,int e){
	int s=0;
	while(o){
		s=s*10+o%10;
		o/=10;
	}
	while(e){
		s=s*10+e%10;
		e/=10;
	}
	return s;
}

int main(){
	int n,e,o;
	printf("enter n:");
	scanf("%d",&n);
	o=split(n,'o');
	e=split(n,'e');
	
	printf("After Even and Odd Seperation:%d\n",merge(o,e));


}
