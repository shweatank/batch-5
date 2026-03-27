/* 
TECHDHABA_SUBMISSION
NAME: <GURUNATH SADAGALA>
EMAIL: <gurunathsadagala@mirafra.com>
PROBLEM_ID: P1 | P2 | P3 
DATE: 2026-01--6
*/

#include<stdio.h>
#include<stdlib.h>

typedef struct operations
{
	int op;
	unsigned int a;
	unsigned int b;
}cal;

typedef struct CRC
{
	int frame;
	int action;
} CRC;

typedef struct CRC_RESULT
{
	int new_frame;
	int status;
}CRC_RESULT;

int add(int a,int b){return a+b; }
int sub(int a,int b){return a-b;}

typedef int (*fun)(int,int);

fun fun_arr[2]={add,sub};

int function_operations(cal var)
{
	return fun_arr[var.op](var.a,var.b);
}

CRC_RESULT crc_function(CRC var)
{

	CRC_RESULT cr;

	var.frame=var.frame^(1<<var.action);

      cr.new_frame=var.frame;

       if(((var.frame>>var.action)&1)==(var.frame>>2&1))
       {
	       cr.status=0;
       }
       else
	       cr.status=-1;

       return cr;
}
void alloc_memory(struct pr2 v)
{
	if(v.cmd==1)
	{
		v21.p[0]=alloc;
		v21.p[0](v);
	}
	else if(v.cmd==2)
	{
		v21.p[1]=free1;
		v21.p[1](v);
	}
}
void  alloc(struct pr2 v)
{
	mem=calloc(1,v.arg);
	printf("memory allocated\n");
}
void (alloc)(struct pr2);
void (free1)(struct pr2);
typedef void (*fptr1)(struct pr2);
struct fn_call
{
	fptr1 p[2];
}v21;
void alloc_memory(struct pr2);
int *mem=NULL;
void free1(struct pr2 v)
{
	if(mem)
	{
		free(mem);
		printf("memory freed\n");
	}
}

int main()
{
       cal var;

       printf("enter the a and b values and option:\n");
       scanf("%d%d%d",&var.a,&var.b,&var.op);

     function_operations(var);

     printf("result=%d\n",function_operations(var));

     CRC cr;
     CRC_RESULT cr_res;

     printf("enter the frame value and action:\n");
     scanf("%d%d",&cr.frame,&cr.action);

     cr_res=crc_function(cr);

     printf("new frame=%d and status=%d\n",cr_res.new_frame,cr_res.status);

     struct pr2 v2
	     printf("enter option and memory size\n");
     scanf(" %hhu %u",&v2.cmd,&v2.arg);
     alloc_memory(v2);
}







	 
