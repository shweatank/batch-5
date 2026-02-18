#include<stdio.h>
void main(){
	int a[5]={1,2,1,2,3},b[100]={0};
	for(int i=0;i<5;i++){
		b[a[i]]++;
		if(b[a[i]]==2)
			printf("%d ",a[i]);
	}
}

