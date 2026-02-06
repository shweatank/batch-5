/* 
TECHDHABA_SUBMISSION
NAME: Siva Sankar
EMAIL: sivasankartelukutla@mirafra.com
PROBLEM_ID: P1 | P2 | P3 | ... | P12
DATE: 2026-02-06
*/




#include<stdio.h>
#include<stdint.h>
  
//P1 — Function Pointer Plugin System (Intermediate)
struct cal{
  uint32_t op;     // 1=op1, 2=op2
  int32_t  a;
  int32_t  b;
};


int32_t operation(struct cal v){
	int add(int a,int b){
		return a+b;
	}
	int sub(int a,int b){
		return a-b;
	}
	int (*fptr[])(int,int)={add,sub};
	int res=fptr[v.op-1](v.a,v.b);
	return res;
}
/*int td_run(const void *in_buf, unsigned int in_len,void *out_buf, unsigned int *out_len){
	struct cal *p=(struct cal*)in_buf;
	int (*fptr[])(int,int)={add,sub};
	int res=fptr[p->op-1](p->a,p->b);
	
}*/
int main(){
	struct cal v;
	scanf("%d %d %d",&v.op,&v.a,&v.b);
	printf("%d\n",operation(v));		
}



//P3 — Bitwise Protocol Parser (Intermediate)
struct bit{
        uint32_t frame;
        uint32_t action;
};

struct bit operation(struct bit b){
        struct bit bo;
        bo=b;
        bo.frame=bo.frame^(1<<2);
        if((bo.frame>>2&1)^(bo.frame>>bo.action&1)==0)
                bo.action=0;
        else
                bo.action=-1;
        return bo;
}

//P6 — Detect Loop in Linked List (Intermediate)
struct sll{
	int n;
	struct sll *link;
};

struct st{
	int has_cycle;
	int entry_index;
}

void detect_loop(struct sll *head,struct st *output){
	struct sll *next,*prev;
	int c=0;
	if(head==head->link){
		output->has_cycle=1;
		output->entry_index=c;
		return;
	}
	next=head;
	while(next->link){
		prev=next->link;
		while(prev->link){
			if(next==prev->link){
				output->has_cycle=1;
				output->entry_index=c;
				return;
			}
			prev=prev->lnik;	
		}
		c++;
		next=next->link;
	}
	output->has_cycle=0;
	output->entry_index=0xffffffff;
}



//P4 — Circular Buffer for Logs (Intermediate)
struct circ_buf{
	int n;
	struct circ_buf *link;
};

struct input{
	
}

void circular_buff(struct circ_buf *head,int )

