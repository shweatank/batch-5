#include <stdio.h>
#include "add.h"
#include "sub.h"
#include "main.h"

void print_result(int add_res, int sub_res)
{
    printf("Addition = %d\n", add_res);
    printf("Subtraction = %d\n", sub_res);
}

int main(void)
{
    int x = 10, y = 5;

    int sum  = add(x, y);
    int diff = sub(x, y);

    print_result(sum, diff);

    return 0;
}

