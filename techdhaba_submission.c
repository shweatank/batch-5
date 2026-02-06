/* 
TECHDHABA_SUBMISSION
NAME: sneha chilvanth
EMAIL:snehachilvanth@mirafra.com
PROBLEM_ID: P1 | P2 | P3 | ... | P12
DATE: 2026-02-06
*/
#include<stdio.h>
int td_init(void);
int td_run(const void *in_buf, unsigned int in_len,
           void *out_buf, unsigned int *out_len);
int td_cleanup(void);
//p1

struct opr {
  uint32_t op;     // 1=op1, 2=op2
  int32_t  a;
  int32_t  b;
};
    int add(int,int);
     int sub(int,int);
     

void main()
{
struct opr v;

printf("enter the option\n");
scanf("%d",v.op);

   printf("enter the numbers\n");
   scanf("%d%d",&v.a,&v.b);
printf("enter the op\n");
scanf("%d",&v.op);

int (*ptr) (int,int);
if(op<0)
printf("invalid op\n");
else if(v.op==1)
{
 int (*ptr)(int,int)=add;
int r=ptr(v.a,v.b);
printf("sum=%d\n",r);
}
else
{
  int (*ptr) (int,int)=sub;
  int r=ptr(v.a,v.b);
  printf("sub=%d\n",r);
  }
  }

  int add(int a,int b)
  {
  return a+b;
  }
 
  int sub(int a,int b)
  {
  return a-b;
  }

//p3

   #include<stdio.h>
   void main()
   {
           int num,pos;
           printf("enter the number\n");
           scanf("%d",&num);
   
           for(pos=31;pos>=0; pos--)
                   printf("%d ",num>>pos&1);
          printf("\n");
          printf("enter the pos to toggle\n");
          scanf("%d",&pos);
          num=num^1<<pos;
  
          for(pos=31;pos>=0; pos--)
                  printf("%d ",num>>pos&1);
          printf("\n");
          if((num>>0&1)==(num>>2&1))
                  printf("bit 0 and bit 2 are equal\n");
          else
                  printf("not equal\n");
 
  }











