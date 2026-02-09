#include <stdio.h>
#include <stdlib.h>

struct Student {
    int id;
    float marks;
};

int main() {
    struct Student *s;

    
    s = (struct Student *)malloc(sizeof(struct Student));
    if (s == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    
    s->id = 101;
    s->marks = 88.5;

    
    printf("ID: %d\n", s->id);
    printf("Marks: %.2f\n", s->marks);

    
    free(s);

    return 0;
}

