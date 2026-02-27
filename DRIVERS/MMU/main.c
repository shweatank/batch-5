#include<linux/slab.h>
#include<linux/vmalloc.h>

void main(){

char *p=kmalloc(126,GPT_KERNEL);

kfree(p);
}
