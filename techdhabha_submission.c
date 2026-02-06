/* 
TECHDHABA_SUBMISSION
NAME: S.PRANEETH
EMAIL: praneethsirimalla@mirafra.com
PROBLEM_ID: P1 | P3 | P6 
DATE: 2026-02-07
*/

//P1 Function Pointer Plugin System 
int func_ptr_plugin(stuct operation *data_ptr)
{
	int(*fptr[2])(int,int)={add,sub};
        return fptr[data_ptr->op](data_ptr->a,data_ptr->b);
}

//P3 — Bitwise Protocol Parser (Intermediate)

/*
#include<stdio.h>
#include<stdlib.h>

struct input
{
        unsigned int frame;
        unsigned int action;
};

struct output
{
        unsigned int new_frame;
        int status;
};
*/
struct output* find_bit_status(struct input *in_data)
{
        int status;
        struct output *out=NULL;
        out=malloc(8);

        (in_data->frame)^=1<<(in_data->action);
        if((((in_data->frame) >>(in_data->action)))&((in_data->frame)>>2))
                status =0;
        else
                status=-1;
        out->new_frame=in_data->frame;
        out->status=status;
        return out;
}
/*
int main()
{
        struct input *in_data=NULL;
        struct output *out_data=NULL;
        in_data=malloc(8);
        in_data->frame=7;
        in_data->action=0;

        out_data=find_bit_status(in_data);
        if(out_data->status==0)
        {
                printf("ok\n");
	}
}*/

//P6 — Detect Loop in Linked List 

struct detect_loop(struct input in) {
        struct input res;
        res.has_cycle = 0;
        res.entry_index = 0xFFFFFFFF;

        struct input *start = in.next+0;
        for(int i = 1;i < in.n;i++) 
	{
                if(in.next+i == start) 
		{
                        res.has_cycle = 1;
                        res.entry_index = i;
                        break;
                }
        }
        return res;
}


