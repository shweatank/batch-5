#include<stdio.h>

void print_bin(char *p){
	for(int i=7;i>=0;i--){
		printf("%d",(*p)>>i&1);
	}
	printf("\n");
}


int main(){
	int n;
	float f;
	char *p=NULL,ch;

	while(1){
		printf("enter choice:");
		scanf("%c",&ch);
		printf("enter value:");
		scanf("%f",&f);
		switch(ch){
			case 'f':
				p=(char*)&f;
				for(int i=0;i<sizeof(f);i++){
					print_bin(p);
					p++;
				}
			//case 'i':


		}
	}
}

