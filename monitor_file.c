#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
   
int main() {
       const char *filename = "/home/mirafra/Desktop/batch-5/monitor_dir/data.txt";
       FILE *file;
       long lastPos = 0;
   
      while (1) {
          file = fopen(filename, "r");
          if (file == NULL) {
              printf("Cannot open file %s\n", filename);
              return 1;
          }
  
          fseek(file, lastPos, SEEK_SET);
  
          char line[1024];
          while (fgets(line, sizeof(line), file)) {
              printf("%s", line);
          }
  
          lastPos = ftell(file);
          fclose(file);
  
          sleep(1);
      }
  
      return 0;
  }
