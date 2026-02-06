/* 
TECHDHABA_SUBMISSION
NAME: <kalisetti likitha>
EMAIL: <likithakalisetti@mirafra.com>
PROBLEM_ID: P1 | P2 | P3 | ... | P12
DATE: 2026-02-06
*/
#include<stdio.h>
int add(int ,int);
int sub(int ,int);
struct ope{
  unsigned int op;     // 1=op1, 2=op2
  int  a;
  int  b;
};
struct plugin
{
        int (*op)(int ,int);
        int (*op1)(int ,int);
};
int main()
{
        struct plugin pl={add,sub};
        struct ope v={1,5,10};
        int res;
        if (v.op == 1)
        res = pl.op(v.a, v.b);
        else
        res = pl.op1(v.a, v.b);
        printf("%d\n",res);
}
int add(int a,int b)
{
        return a+b;
}
int sub(int a,int b)
{
        return a-b;
}

//////////////////////////////////////

#include<stdio.h>
struct input{
    unsigned int frame;
    unsigned int action;
};
struct output {
    unsigned int new_frame;
    unsigned int  status;
};
int main()
{
        struct input in;
        int pos;
        unsigned int res;
        in.frame=15;
        in.action=2;
        res=in.frame;
                res=res^(1<<in.action);

        struct output ou;
        ou.new_frame=res;
        if(in.frame>>in.action&1==in.frame>>2&1)

                ou.status=0;
        else
                ou.status=-1;
        printf("Status:%d\n",ou.status);
}


