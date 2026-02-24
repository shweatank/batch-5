#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE *checkErrors(int argc,char *argv[]) {
	if(argc != 2) {
                printf("Invalid Arguments\n");
                exit(0);
        }
        if(strlen(strchr(argv[1],'.')) == 2 && argv[1][(strchr(argv[1],'.') - argv[1]) + 1] != 'c') {
                printf("Invalid input file extension\n");
                exit(0);
        }
        FILE *fp = fopen(argv[1],"r");
        if(fp == NULL) {
                printf("Input file not found\n");
                exit(0);
        }
	return fp;
}

char * destinationFileCreation(char *srcFile) {
    int len = strlen(srcFile);
    char *destFileName = malloc(len + 1); // +1 for '\0'
    strncpy(destFileName, srcFile,);
    destFileName[len-1] = 'i';
    return destFileName;
}

void myPreprocessor(int argc,char *argv[]) {
	FILE *fp = checkErrors(argc,argv); // Checking for the Errors during inputs and returns the file pointer
	char *destFile = destinationFileCreation(argv[1]); // Creation of destination file name same as src file name with .i extension
	FILE *dest = fopen(destFile,"w");

	ssize_t noOfChars = 0;       // number of characters read
    	while(noOfChars != -1) {
		char *line = NULL;   // getline() will allocate memory
    		size_t len = 0;      // buffer size (auto managed by getline)
    		noOfChars = getline(&line, &len, fp);
		int temp = strstr(line,"//")-line;
		line[temp] = '\0';

			
		




