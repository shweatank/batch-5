#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void myCopyCommand(int argc,char *files[]) {
	if(argc < 2) {
		printf("Invalid arguments\n");
		exit(0);
	}
	FILE *src1 = fopen(files[1],"r");
	FILE *src2 = fopen(files[2],"r");
	if(src1 == NULL || src2 == NULL) {
		if(src1 == NULL && src2 != NULL) 
			printf("%s Source file not found\n",files[1]);
		else if(src2 == NULL && src1 != NULL) 
			printf("%s Source file not found\n",files[2]);
		else 
			printf("%s %s Source files not found\n",files[1],files[2]);
		exit(0);
	}
	FILE *dest = fopen(files[3],"w");
	while (1) {
		char word1[100],word2[100];
        	int r1 = fscanf(src1,"%s",word1);
        	int r2 = fscanf(src2,"%s",word2);

        	if(r1==1) fprintf(dest,"%s ",word1);
        	if(r2==1) fprintf(dest,"%s ",word2);

        	if(r1!=1 && r2!=1) break;
   	 }

    	fclose(src1);
    	fclose(src2);
    	fclose(dest);

}

int main(int argc,char *argv[]) {
	myCopyCommand(argc,argv);
}
