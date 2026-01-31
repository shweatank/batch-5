#include <stdio.h>
#include <unistd.h>

int main() {
    FILE *fp = fopen("data.txt", "r");
    if (!fp) {
        perror("fopen");
        return 1;
    }

    char buf[128];
   // fseek(fp, 0, SEEK_END);  

    while (1) {
        while (fgets(buf, sizeof(buf), fp)) {
            printf("%s", buf);
            fflush(stdout);
        }
        clearerr(fp);        
        sleep(1);           
    }

    fclose(fp);
    return 0;
}

