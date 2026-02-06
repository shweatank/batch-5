/*TECHDHABA_SUBMISSION
NAME: Jagilinki Sai Kumar
EMAIL: jgilinkisaikumar@mirafra.com
PROBLEM_ID: P1 | P2 | P3
DATE: 06-02-2025 */

#include<stdio.h>
#include<stdlib.h>
struct in {
unsigned int op;
int a;
int b;
};
int sum(int,int);
int sub(int,int);
typedef int  (*fptr)(int,int);
struct ptr
{
fptr p1;
fptr p2;
};
struct in in_buf;
struct ptr v;
struct pr2
{
unsigned char cmd;
unsigned int arg;
};
void (alloc)(struct pr2);
void (free1)(struct pr2);
typedef void (*fptr1)(struct pr2);
struct fn_call
{
fptr1 p[2];
}v21;
void alloc_memory(struct pr2);
int *mem=NULL;
typedef struct CRCI
{
int frame;
int action; // bit0 => toggle flag bit2
}CRC_IN;

typedef struct CRCO
{
int new_frame;
int  status;
}CRC_OUT;
void crc_action(CRC_IN);

typedef struct stack
{
u32 cmd_count;
  u8 cmd;    // 1=PUSH, 2=POP
  u8 len;    // for PUSH only
  u8 data[len];
}STK_IN;
typedef struct stack
{
u32 pop_count;
  u8 len;
  u8 data[len];
}STK_OUT;
STK_IN s1;
STK_OUT s2;
void main()
{
/*problem 1*/

/*printf("enter numbers\n");
scanf("%d%d",&in_buf.a,&in_buf.b);
printf("enter option\n");
scanf("%d",&in_buf.op);
v.p1=sum;
v.p2=sub;
if(in_buf.op==1)
printf("add=%d\n",v.p1(in_buf.a,in_buf.b));
else if(in_buf.op==2)
printf("sub=%d\n",v.p2(in_buf.a,in_buf.b));*/

/*problem 2*/

/*(struct pr2 v2;
printf("enter option and memory size\n");
scanf(" %hhu %u",&v2.cmd,&v2.arg);
alloc_memory(v2);*/

/*problem 3*/
/*CRC_IN v1;
printf("enter number and action\n");
scanf("%d %d",&v1.frame,&v1.action);
crc_action(v1);*/

/*problem 4*/




}
void crc_action(CRC_IN v)
{
CRC_OUT v2;
v.frame^=(1<<v.action);
if((v.frame>>v.action)&1==((v.frame>>2)&1))
{
v2.new_frame=v.frame;
v2.status =0;
}
else
{
v2.new_frame=v.frame;
v2.status=-1;
}
printf("%d %d\n",v2.new_frame,v2.status);
}
int sum(int a,int b)
{
return a+b;
}
int sub(int a,int b)
{
return a-b;
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
void free1(struct pr2 v)
{
if(mem)
{
free(mem);
printf("memory freed\n");
}
}
