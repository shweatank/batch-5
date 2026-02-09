#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define N 2
#define ROW 2
#define COL 3

void * input(int ***a){
	srand(getpid());
	a=malloc(N*sizeof(int*));
	for(int i=0;i<N;i++){
		a[i]=malloc(ROW*sizeof(int*));
		for(int j=0;j<ROW;j++){
			a[i][j]=malloc(COL*sizeof(int));
			for(int k=0;k<COL;k++){
				a[i][j][k]=rand()%40+30;
			}
		}
	}
	return a;
}

void print(int ***a){
	for(int i=0;i<N;i++){
		for(int j=0;j<ROW;j++){
			for(int k=0;k<COL;k++){
				printf("%d ",a[i][j][k]);
			}
			printf("\n");
		}
		printf("\n\n");
	}
}

int main(){
	int ***a=NULL;
	a=input(a);
	print(a);
}

