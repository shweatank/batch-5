#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int global_var = 100;   

int main()
{
    int pipe1[2];
    int pipe2[2];

    int a=10,b=5;
    int sum,sub;

    pipe(pipe1);
    pipe(pipe2);
    
    pid_t pid= fork();

    if (pid>0) {

       
        printf("Process X - global_var address: %p\n", (void *)&global_var);

        close(pipe1[0]);
        close(pipe2[1]);

        write(pipe1[1], &a, sizeof(a));
        write(pipe1[1], &b, sizeof(b));

        read(pipe2[0],&sum, sizeof(sum));
        printf("Process X receive sum :%d\n", sum);

        sub= a-b;
        write(pipe1[1], &sub, sizeof(sub));
    
    } else {

       
        printf("Process Y - global_var address: %p\n", (void *)&global_var);

        close(pipe1[1]);
        close(pipe2[0]);

        read(pipe1[0],&a, sizeof(a));
        read(pipe1[0],&b, sizeof(b));

        sum = a+b;
        write(pipe2[1], &sum,sizeof(sum));

        read(pipe1[0],&sub, sizeof(sub));
        printf("Process Y received subtratction: %d\n",sub);
    }

    return 0;
}

