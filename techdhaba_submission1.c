/*
TECHDHABA_SUBMISSION
NAME : Vennapusa Ramesh Reddy
EMAIL:vennapusarameshreddy6@gmail.com
PROBLEM_ID: p1,p3
DATE :2026-02-06
///////////////p1///////////
#include<stdio.h>
struct op
{
        unsigned int op;
        int a;
        int b;
};

int add(int n1,int n2)
{
        return n1+n2;
}

int sub(int n1,int n2)
{
        return n1-n2;
}

int funcplugin(struct op *ptr)
{
        int (*fptr[2])(int,int)={add,sub};
        return fptr[ptr->op](ptr->a,ptr->b);
}
int main()
{
        struct op data;
        data.a=10;
        data.b=20;
       printf("enter the op\n");
       scanf("%d",&data.op);
       printf("result :%d\n",funcplugin(&data));
}
///////////////p3///////////
#include <stdio.h>

typedef unsigned int u32;
struct CRC {
    u32 frame;
    int action;
};

struct CRC_res {
    int status;
    u32 new_frame;
};


struct CRC_res P3(struct CRC v){
    struct CRC_res res;
    u32 a = v.frame, b,c,d;

    b = a ^ (1 << v.action);
    c=b&(1<<v.action);
    d=a&(1<<2); 
    if (c ==d)
        res.status = 0;
    else
        res.status = -1;
    res.new_frame = b | v.frame;
    return res;
}

int main()
{
    struct CRC crc;
    struct CRC_res result;
    crc.frame = 1;   
    crc.action = 0;  
    result = P3(crc);
    printf("Status: %d\n",result.status);
    printf("New frame: %u\n",result.new_frame);
    return 0;
}
///p6/////////////////////////
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
}*/
        



