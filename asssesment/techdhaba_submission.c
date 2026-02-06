/* 
TECHDHABA_SUBMISSION
NAME: <Badana Sai Kumar>
EMAIL: <saikumarbadana@mirafra.com>
PROBLEM_ID: P1 | P3 | P6 
DATE: 2026-02-06
*/

#include<stdint.h>
#include<stddef.h>
#include<stdio.h>


//P1
struct cal {
  uint32_t op;     // 1=op1, 2=op2
  int32_t  a;
  int32_t  b;
};

int32_t Function_pointer(struct cal );
int32_t add(int a,int b);
int32_t sub(int a,int b);
int main()
{
	struct cal c={2,20,20};
	Function_pointer(c);
}
int32_t Function_pointer(struct cal c)
{
	int32_t add(int a,int b)
        {
                 return a+b;
        }
        int32_t sub(int a,int b)
        {
                 return a-b;
        }
	int32_t result;
	int32_t (*ptr[3])(int,int);
	ptr[0]=NULL;
	ptr[1]=add;
	ptr[2]=sub;
	result=ptr[c.op](c.a,c.b);
	return result;
}


//P3
struct c
{
        uint32_t frame;
        uint32_t action;
};
struct c Bit_parser(struct c );
struct c Bit_parser(struct c c)
{
        struct c new;
        new.frame=c.frame^(1<<2);
        if(((new.frame>>2)&1) ^ ((c.frame>>c.action)&1))
        {
                new.action=-1;
        }
        else
        {
                new.action=0;
        }
        return new;
}




//P6

struct new
{
	int has_cycle;  // 0/1
	int entry_index;
};

struct new Loop(struct sll *head)
{
	struct new output;
	int cnt=0;
	struct sll *slow=head,*fast=head->next;
	while(fast&&fast->next)
	{
		cnt++;
		slow=slow->next;
		fast=fast->next->next;
		if(slow==fast)
		{
			break;
		}
	}
	if(slow==fast)
	{
		output.has_cycle=1;
		output.entry_index=cnt;
	}
	else
	{
		output.has_cycle=0;
		output.entry_index=0xFFFFFFFF;
	}
	return output;
}



