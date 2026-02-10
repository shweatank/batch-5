#include<stdio.h>
 
/*int main(){
	int n,arr[]={1,2,2,3,4,4,5,5},i,j;
	n=sizeof(arr)/sizeof(int);
	for(i=0;i<n;i++){
		for(j=i+1;j<n;j++){
			if(arr[i]==arr[j])
				memmove(arr+i,arr+j,sizeof(int)*(n-i));
		}
	}
	i=0;
	while(i<n)
		printf("%d,",arr[i++]);
}*/
int main(){
	int n,i,j,arr[]={1,2,2,3,4,4,5,5};
	n=sizeof(arr)/sizeof(int);
	int u[n];
	for(i=0;i<n;i++){
		if(arr[i]!=arr[i+1])
			u[j++]=arr[i];
	}
	i=0;
        while(i<j)
                printf("%d,",u[i++]);
}
	
