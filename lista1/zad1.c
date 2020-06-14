#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Node{
    struct Node* next;
    int val;
};

void add(int val, struct Node* fifo){
    if (fifo->next != NULL){
        add(val,fifo->next);
    } else{
        fifo->val = val;
        struct Node *test = (struct Node*)malloc(sizeof(struct Node));
        fifo->next = test;
    }
}
int pop(struct Node* fifo){
    if (fifo->next != NULL){

        int ret = fifo->val;
        struct Node* del = fifo;
        *fifo = *fifo->next;
        return ret;
    }
    return -1;
}

int main() {
    struct Node *test = (struct Node*)malloc(sizeof(struct Node));
    add(1,test);
    add(2,test);
    int a = pop(test);
    int b = pop(test);
    int c = pop(test);

    printf("%d %d %d",a,b,c);
    return 0;
}

