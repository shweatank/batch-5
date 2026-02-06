/* 
TECHDHABA_SUBMISSION
NAME: Varshitha S
EMAIL: svarshitha@mirafra.com
PROBLEM_ID: P1 | P2 | P3 | ... | P12
DATE: 2026-02-06
*/
int td_init(void);
int td_run(const void *in_buf, unsigned int in_len,
           void *out_buf, unsigned int *out_len);
int td_cleanup(void);
//p1
int add(int a,int b)
{return a+b;}
int sub(int a,int b)
{return a-b;}
int td_run(const void *in_buf,unsigned int in_len,void *out_buf,unsigned int *out_len)
{
int i=*(int *)in_buf;
int res,a=*(int *)(in_buf+4),b=*(int *)(in_buf+8);
int (*ptr[2])(int,int);
ptr[0]=add;
ptr[1]=sub;
if(i==2||i==1)
{
res=ptr[i-1](v.a,v.b);
return res;
}
return 0;
}

//p3

int td_run(const void *in_buf,unsigned int in_len,void *out_buf,unsigned int *out_len)
{
unsigned int frame=*(int *)in_buf,action=*(int *)(in_buf+4);
frame=frame^(1<<action);
*(int *)out_buf=frame;
if(((frame>>action)&1)==((frame>>2)&1))
{
*(int *)(out_buf+4)=0;
return 0;
}
else
{
*(int *)(out_buf+4)=-1;
return -1;
}
}

//p6
int td_run(const void *in_buf,unsigned int in_len,void *out_buf,unsigned int *out_len)
{
struct *head;
struct *first=head,*second=head;

while(first!=0&&second!=0&&second->next!=0)
{
first=first->next;
second=second->next->next;
if(first==second)
return 1;
}
return 0;
}


