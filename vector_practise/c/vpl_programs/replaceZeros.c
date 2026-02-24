#include <stdio.h>
int replaceZeros(int);
int main() {
    int num;
    scanf("%d",&num);
    int res = replaceZeros(num);
    //printf("%d",res);
    //if(res < 0) printf("%d\n",num);
    //else{
    (res == num) ? printf("no zeros in input") : (num < 0) ? printf("-%d\n",res) : printf("%d\n",res);
//	}
}

int replaceZeros(int num) {
    int res = 0,temp,multiply = 1;
    if(num == 0) return 1;
    num = num;if(num < 0) num = -num;
    while(num) {
        temp = num % 10;
	//res = temp;
	//num /= 10;
	//continue;
        if(!temp) res += 1*multiply;
        else res += multiply*temp;
        num /= 10;
	multiply *= 10;
    }
    return res;
}
