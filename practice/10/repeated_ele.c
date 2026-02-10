#include<stdio.h>
#include<limits.h>

void main(){
	int c=0;
	int a[15]={1,1,1,2,2,3,3,3,3,4,5,6,7,8,8};
	int p[1000]={0};
	for(int i=0;i<15;i++){
		p[a[i]]++;
		if(p[a[i]]==2)
			printf("%d ",a[i]);
	}
printf("\nPID=%ld\n",getpid());
FILE *fp1,*fp2,*fp3;
fp1=fopen("data1","w+");
fp2=fopen("data2","w+");
fp3=fopen("data3","w+");
int id=getpid();
printf("%s\n",system("ls /proc/id/fd"));
while(1);
}
