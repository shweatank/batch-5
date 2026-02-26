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
	else
		printf("crc fail");
}
