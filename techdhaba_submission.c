/*
NAME: K Jugal Kishore
EMAIL: jugalkishore@mirafra.com
PROBLEM_ID: P1|P3|P6
DATE: 2026-02-06
*/

#include<stdio.h>
typedef unsigned int u32;
typedef char u8;
typedef int (*cal)(int,int);
int add(int,int);
int sub(int,int);
struct st {
	u32 op;
	u32 a;
	u32 b;
};
struct CRC{
	u32 frame;
	u32 action;
};
struct CRC_res{
	u32 new_frame;
	int status;
};

struct DS{
	u32 n;
	struct DS *next;
};

struct DS_res{

	u32 has_cycle;
	u32 entry_index;
};

int P1(struct st);
struct CRC_res P3(struct CRC);
struct DS_res P6(struct DS*);
struct DS *head=NULL;
void main(){
	struct st v;
	printf("Enter operation 1)add 2)sub\n");
	scanf("%d",&v.op);
	printf("Enter two numbers\n");
	scanf("%d%d",&v.a,&v.b);
	printf("result=%d\n",P1(v));
	struct CRC_res v1;
	struct CRC v2;
	printf("Enter the frame and action\n");
	scanf("%d%d",&v2.frame,&v2.action);
	v1=P3(v2);
	printf("P3 result=%d\n",v1.status);
}

int P1(struct st v){
	cal cal1;
	cal1=v.op?add:sub;
	return cal1(v.a,v.b);
}

int add(int a,int b){return a+b;}
int sub(int a,int b){return a-b;}

struct CRC_res P3(struct CRC v){
	struct CRC_res res;
	u32 a=v.frame,b;
	b=a^(1<<v.action);
	if((b&(1<<v.action))==(a&(1<<2)))
		res.status=0;
	else
		res.status=-1;
	res.new_frame=b|v.frame;
	return res;
}

struct DS_res P6(struct DS *head){
	struct DS res;
	struct DS *fast,*slow,*temp=head;
	if(head && head->next)
		slow=head,fast=head->next;
	int c=0,flag=1;
	while(slow!=fast){
		slow=slow->next;
		fast=fast->next;
		if(slow->next==NULL || fast->next==NULL){
			flag=0;
			break;
		}
	}
	if(flag)
		while(slow!=temp){
			temp=temp->next;
			c++;
		}
	if(flag){
		res.has_cycle=1;
		res.entry_index=c;
	}
	else{
		res.has_cycle=0;
		res.entry_index=0xFFFFFFF;
	}
	return res;
}









