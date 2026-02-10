#include<stdio.h>
int main(){
	int num=30,i=0,f;
	while(num>0){
		num>>=i;
		if(num&1)
			f=i;
		else
			break;
		i++;
	}
	num=num^(1<<f);
	/*int n=30,pos=0;
	while ( ((n >> pos) & 1) == 0 ) {
        	pos++;
    	}
	n=n^(1<<pos);*/
	printf("num is %d\n",num); 
}






