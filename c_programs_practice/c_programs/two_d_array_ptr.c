#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define ROW 3 
#define COL 3

void *input(int **a){
	srand(getpid());
	a=malloc(ROW*sizeof(int*));
	for(int i=0;i<COL;i++){
		a[i]=malloc(COL*sizeof(int));
		for(int j=0;j<COL;j++){
			a[i][j]=rand()%7+6;
		}	
	}
	return a;
}

void print(int **a){
	for(int i=0;i<ROW;i++){
		for(int j=0;j<COL;j++){
			printf("%d ",a[i][j]);
		}
		printf("\n");
	}
}

int main(){
	int **a=NULL;
	a=input(a);
	print(a);
}

