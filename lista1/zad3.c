#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Ring{
    struct Ring* next;
    struct Ring* before;
    int val;
};

void add(int val, struct Ring* ring){
    struct Ring* newElement = (struct Ring*) malloc(sizeof(struct Ring));
    if (ring->next == NULL){
        ring->next = ring;
        ring->before = ring;
        ring->val = val;
        return;
    }
    struct Ring* last = ring->before;
    newElement->next = ring;
    newElement->before = last;
    last->next = newElement;
    newElement->val = val;
    ring->before = newElement;
}
int get(struct Ring* ring,int n){
    if (ring->next == NULL){
        return -1;
    }
    while (n > 0){
        n--;
        ring = ring->next;
    }
    while (n < 0){
        n++;
        ring = ring->before;
    }
    return ring->val;
}

void delete(struct Ring* ring,int n){
    while (n > 0){
        n--;
        ring = ring->next;
    }
    while (n < 0){
        n++;
        ring = ring->before;
    }
    struct Ring* toDelete = ring;
    struct Ring* before = ring->before;
    struct Ring* after = ring->next;
    before->next = after;
    after->before = before;
    free(toDelete);
}

void merge(struct Ring* l1, struct Ring* l2){
    struct Ring* lastL1 = l1->before;
    struct Ring* lastL2 = l2->before;
    lastL1->next = l2;
    lastL2->next = l1;
    l1->before = lastL2;
    l2->before = lastL1;
}

int main() {
    srand(time( NULL ));
    struct Ring *test = (struct Ring*)malloc(sizeof(struct Ring));
//    add(0,test);
//    add(1,test);
//    add(2,test);
//    delete(test,1);
//    printf("%d %d %d %d",get(test,0),get(test,1),get(test,2),get(test,-1));
    for (int i = 0; i < 1000; ++i) {
        add(rand()%50,test);
    }
    clock_t start = clock();
    for (int j = 0; j < 100; ++j) {
        get(test,rand() % 1000);
    }
    start = (clock() - start);
    printf("Losowe elementy: %f\n",(float)start/ CLOCKS_PER_SEC);

    start = clock();
    for (int j = 0; j < 100; ++j) {
        get(test,399);
    }
    start = (clock() - start);
    printf("Ten sam element: %f\n",(float)start/ CLOCKS_PER_SEC);

    struct Ring *l1 = (struct Ring*)malloc(sizeof(struct Ring));
    struct Ring *l2 = (struct Ring*)malloc(sizeof(struct Ring));
    add(1, l1);
    add(2, l1);
    add(3, l2);
    add(4, l2);
    merge(l1,l2);
    printf("%d %d %d %d",get(l1,0),get(l1,1),get(l1,2),get(l1,-1));
    return 0;
}

