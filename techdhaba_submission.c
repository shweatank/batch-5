#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
struct st{
  uint32_t op;     // 1=op1, 2=op2
  int32_t  a;
  int32_t  b;
};
struct crc_in
{
uint32_t frame;
uint32_t action; // bit0 => toggle flag bit2
};
struct crc_out
{
uint32_t new_frame;
int32_t  status; // 0 ok, -1 CRC fail
};
struct crc_out toggle_func(struct crc_in);
int32_t add(int32_t x,int32_t y){return x+y;}
int32_t sub(int32_t x,int32_t y){return x-y;}
int32_t operation(struct st var)
{
	int32_t (*fp[2])(int32_t,int32_t)={add,sub};
	return fp[var.op](var.a,var.b);
}
struct crc_out toggle_func(struct crc_in v)
{
	struct crc_out res;
	uint8_t b0=(uint8_t)(v.frame&0xFF);
	uint8_t b1=(uint8_t)((v.frame>>8) & 0xFF);
	uint8_t b2=(uint8_t)((v.frame>>16) & 0xFF);
	uint8_t b3_crc=(uint8_t)((v.frame>>24) & 0xFF);
	uint8_t crc=b0^b1^b2;
	if(b3_crc!=crc)
	{
		res.status=-1;
		res.new_frame=v.frame;
		return res;
	}
	if(v.action & 0x01)
	{
		v.frame^=(1<<2);
		b0=(uint8_t)(v.frame&0xFF);
		crc=b0^b1^b2;
		res.new_frame=((uint32_t)crc<<24)|(v.frame & 0x00FFFFFF);
	}
	else
	{
		res.new_frame=v.frame;
	}
	res.status=0;
	return res;
}


struct loop_in{
    uint32_t n;
    struct loop_in *next;
};

struct loop_out{
    uint32_t has_cycle;
    uint32_t entry_index;
};
struct loop_out find_loop(struct loop_in *head)
{
	struct loop_out res;
	int cnt=0;
	struct loop_in *slow=head,*fast=head;
	while(fast && fast->next)
	{
		slow=slow->next;
		fast=fast->next->next;
		if(slow==fast)
		{
			res.has_cycle=1;
			slow=head;
			while(slow!=fast)
			{
				slow=slow->next;
				fast=fast->next;
				cnt++;
			}
			res.entry_index=cnt;
			return res;
		}
		
	}
	res.has_cycle=0;
	res.entry_index=0xFFFFFFFF;
	return res;
}

