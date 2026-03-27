#include<stdio.h>

char** func1_Str();
char** func2_Str();

char** func1_Str()
{
  static char *p = "Linux";
  return &p;
}

char** func2_Str()
{
  static char *p = "Windows";
  return &p;
}

static void function()
{
	printf("hello world");
}
