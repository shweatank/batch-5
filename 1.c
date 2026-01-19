#include"header.h"
void* thread_1(void*p){
	printf("In thread\n");
}
void main(){
	pthread_t t1;
	pthread_create(&t1,0,thread_1,0);
	printf("in main\n");
while(1);
}
