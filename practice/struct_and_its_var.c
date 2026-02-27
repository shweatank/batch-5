#include<stdio.h>
#include<stdlib.h>
typedef struct rectangle
{
	float len;
	float bred;
}rect;
float find_area(rect var)
{
	return var.len*var.bred;
}
int main()
{
	rect var;
	var.len=2.0;
	var.bred=3.0;
	float area=find_area(var);
	printf("area:%0.2f\n",area);
}
