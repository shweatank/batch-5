#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void copyAlternateWords(int argc,char *argv[]) {
	FILE *src1 = fopen(argv[1],"r");
	FILE *src2 = fopen(argv[2],"r");

