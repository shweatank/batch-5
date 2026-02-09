#include<stdio.h>
struct input
{
	unsigned int cmd_cnt,rep_cmd_cnt;
	unsigned char cmd;
	unsigned int arg;
};

struct output
{
	unsigned int resp_cnt,rep_cnt,handle_or_zero;
	int status;
};


int main()
{
	struct input *in=NULL;
	struct output *out=NULL;
}
