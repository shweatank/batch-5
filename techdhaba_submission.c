/*TECHDHABA_SUBMISSION
NAME: <Narala Kavyanjali>
EMAIL: <kavyanjalinarala@mirafra.com>
PROBLEM_ID: P1 | P3 | P4 | p6
DATE: 2025-02-06*/

#include<stdio.h>
#include<stdlib.h>
typedef unsigned int u32
typedef unsigned char u8

struct vals{
	u32 op;
	int a;
	int b;
};

struct func_ops{
	int (*add_nums)(struct vals);
	int (*sub_nums)(struct vals);
};

int add_nums(struct vals p){
	return p.a+p.b;
}
int sub_nums(struct vals p){
	return p.a-p.b;
}

struct ip{
	u32 frame;
	u32 action;	
};

struct op{
	u32 new_frame;
	u32 status;
};

struct op comput(struct ip x){
	struct op s;
	x.frame^=(x.frame<<x.action);
	if(((x.frame>>2)&1)==((x.frame>>x.action)&1))
		s.status=0;
	else 
		s.status=-1;
	s.new_frame=x.frame;
return s;
}


struct input{
	u32 cmd_count;
	u8 cmd;
	u8 len;
	u8 data[len];
};

struct output{
	u32 pop_count
	repeat pop_count:
  	u8 len
  	u8 data[len]

}
struct output message_buff(struct input x){
	int rem=0,used=0,head=0,tail=0;
	if(x.cmd=1){
		while(cmd_count<=5){
		puts("give length of the message to enter");
		scanf("%d",&x.len);
		s=len+1;
		if(rem>len){
			fgets(&x.data[len+1],len,stdout);
			head=len+l;
		}
		else{
			while(used+len+1>32){
				int old_len=data[head];
				head=(head+1+old_len)
				used=used-	
		}
		rem=32-len;
	}
	if(x.cmd==2){
	
	}
		}
	}
}

struct nodes_ip{
	u32 n;
	struct DS *next;
};

struct nodes_op{
	u32 has_cycle;
	u32 entry_index;
};

struct nodes_ip node_find(struct DS *head){
	struct nodes_ip res;
	struct nodes_op *fast,*slow,*temp=head;
	if(head && head->next)
		slow=head,fast=head->next;
	int c=0,f=1;
	while(slow!=fast){
		slow=slow->next;
		fast=fast->next;
		if(slow->next==NULL || fast->next==NULL){
			f=0;
			break;
		}
	}
	if(f)
		while(slow!=temp){
			temp=temp->next;
			c++;
		}
	if(f){
		res.has_cycle=1;res.entry_index=c;
	}
	else{
		res.has_cycle=0;res.entry_index=0xFFFFFFF;
	}
	return res;
}

int main(){
	struct vals c={1,5,6};
	struct func_ops op={.add_nums=add_nums,.sub_nums=sub_nums};
	int res=(c.op==1)?op.add_nums(c):op.sub_nums(c);
	printf("%d",res);
	struct ip p={1,0};
	struct op x=comput(p);
	printf("frame %b, status %d",x.new_frame,x.status);
}






	       
