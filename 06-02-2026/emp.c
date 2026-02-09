/* 
ABHISHEK_SUBMISSION
NAME:Abhishek
DATE:2026-02-06
*/

/*p1:
#include<stdio.h>
struct input {
    unsigned int op;  
    int a;
    int b;
};

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }

int main()
{
    struct input in;
    int result;
    int (*fptr[2])(int,int) = { add, sub };
    in.op = 1;
    in.a = 10;
    in.b = 20;
    result = fptr[in.op - 1](in.a, in.b);
    printf("Result: %d\n", result);

    in.op = 2;
    in.a = 50;
    in.b = 15;
    result = fptr[in.op - 1](in.a, in.b);
    printf("Result: %d\n", result);

    return 0;
}*/

/*p3
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
    u32 a = v.frame, b;

    b = a ^ (1 << v.action);

    if ((b & (1 << v.action)) == (a & (1 << 2)))
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

    printf("Status: %d\n", result.status);
    printf("New frame: %u\n", result.new_frame);

    return 0;
}
*/

/*p6
struct Result p_fun(struct List input) {
    struct Result r;
    r.has_cycle = 0;
    r.entry_index = 0xFFFFFFFF;

    struct Node *head = input.nodes;

    for (int i = 1; i < input.size; i++) {
        if (&input.nodes[i] == head) {
            r.has_cycle = 1;
            r.entry_index = i;
            break;
        }
    }

    return r;
}

*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/ioctl.h>
#define DEVICE_NAME "p10char"
#define CLASS_NAME "p10"
#define IOCTL_RESET _IO

