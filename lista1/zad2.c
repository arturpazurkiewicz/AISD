#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct List {
    int val;
    struct List *next;
};

void add(struct List *list,int val) {
    if (list->next != NULL) {
        add(list->next,val);
    } else {
        list->val = val;
        struct List *test = (struct List *) malloc(sizeof(struct List));
        list->next = test;
    }
}

int size(struct List *list) {
    int counter = 0;
    while (list->next != NULL) {
        list = list->next;
        counter++;
    }
    return counter;
}

int get(struct List *list, int number) {
    while (number > 0) {
        number--;
        list = list->next;
    }
    return list->val;
}

void delete(struct List *list, int number) {
    while (number > 1) {
        number--;
        list = list->next;
    }

    list->next = list->next->next;
}

void merge(struct List *l1, struct List *l2) {
    while (l1->next != NULL) {
        l1 = l1->next;
    }
    l1->next = l2->next;
};

int main() {
    struct List *l1 = (struct List *) malloc(sizeof(struct List));
    add(l1, 1);
    add(l1, 2);
    add(l1, 3);
    struct List *l2 = (struct List *) malloc(sizeof(struct List));
    add(l2, 4);
    add(l2, 5);
    merge(l1, l2);
   // printf("%d ", size(l1));
    struct List *test = (struct List *) malloc(sizeof(struct List));

    for (int i = 0; i < 1000; ++i) {
        add(test,i);
    }
    clock_t t1=0;
    clock_t t2=0;
    clock_t start = clock();
    for (int j = 0; j < 100; ++j) {
        get(test,rand() % 1000);
    }
    t1 = (clock() - start);
    start = clock();
    for (int j = 0; j < 100; ++j) {
        get(test,399);
    }
    t2 = (clock() - start);

    printf("losowy: %f ten sam: %f\n", (float)t1/ CLOCKS_PER_SEC, (float)t2/ CLOCKS_PER_SEC);


    return 0;
}

