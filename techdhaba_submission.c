/* 
TECHDHABA_SUBMISSION
NAME: <Ajay Ganji>
EMAIL: <ajayganji@mirafra.com>
PROBLEM_ID: P1 | P3 | P6
DATE: 2026-02-06
*/



#include <stdio.h>
#include <stdlib.h>

typedef unsigned int 	u32;
typedef signed int 	s32;
typedef unsigned char	u8;

/*
struct p1 {
	u32 op;
	s32 a;
	s32 b;
};

int add(int a,int b) { return a+b; }
int sub(int a,int b) { return a-b; }
*/

int p1_fun(struct p1 in_buf) {
	int (*fptr[2])(int,int) = {add,sub};
	if(in_buf.op > 1) { exit(0); }
	return fptr[in_buf.op](in_buf.a,in_buf.b);
}

struct p3_crc_res p3_fun(struct p3_crc v){
	struct p3_crc_res res;
	u32 a=v.frame,b;
	b=a^(1<<v.action);
	if((b&(1<<v.action))==(a&(1<<2)))
		res.status=0;
	else
		res.status=-1;
	res.new_frame=b|v.frame;
	return res;
}

struct p6_res p6_fun(struct p6_input input) {
	struct p6_res res;
	res.has_cycle = 0;
	res.entry_index = 0xFFFFFFFF;
	
	input *start = input.next+0;
	for(int i = 1;i < input.n;i++) {
		if(input.next+i == start) {
			res.has_cycle = 1;
		        res.entry_index = i;
			break;
		}
	}
	return res;
}



int main() {
	struct p1 data = {0,2,3};
	printf("result: %d\n",p1_fun(data));
}


	
