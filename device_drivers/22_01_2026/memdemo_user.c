#include<stdio.h>
#include<stdlib.h>
#include<linux/slab.h>
#include<linux/gfp.h>

int main(){
	int *a=kmalloc(sizeof(int),GFP_USER);
	printf("enter:");
	scanf("%d",a);
}

