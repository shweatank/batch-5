#include<stdio.h>
int main(){
	char kernel_buffer[]="10,20,+";
int a[5],j=0,h=0,f=0,i=0,num[3]={0},k=0,m=0;
char ch;
while(kernel_buffer[i]!='\0'){
                if(kernel_buffer[i]==','){
                        a[j++]=i;
                }
        i++;
        }
     i=0;
    while(kernel_buffer[i]!= '\0'){
            if(kernel_buffer[i]==','){ 
		    i++;
	    }
	    if(i-1==a[j-1]){
                     ch=kernel_buffer[i];
		     f++;
	    }
	    else{
            		while(kernel_buffer[i]!=','){
		    		num[k]=num[k]*10+(kernel_buffer[i]-'0');
		    		i++;
            }
			f++;
			k++;
	    }
	if(f==3)
		break;
	}
	    
    
    int res=0;
    switch(ch){
            case '+':res=num[0]+num[1];
                     break;
            case '-':res=num[0]-num[1];
                        break;
            case '*':res=num[0]*num[1];
                        break;
           case '/':res=num[0]/num[1];
                        break;
           case '%':res=num[0]%num[1];
                        break;
        default : printf("No valid input operator\n");
    }
        printf("result is %d\n",res);
}
