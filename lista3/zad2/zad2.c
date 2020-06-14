#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "pcg_basic.h"


void list_printing(int *array, int p, int q) {
    for(int i = p; i <= q; i++) {
        fprintf(stderr, "%d ", array[i]);
    }
    fprintf(stderr, "\n");
}

void swap(int* a, int* b, int* moves) {
    fprintf(stderr, "swapping %d with %d\n", *a, *b);
    (*moves)++;
    int t = *a;
    *a = *b;
    *b = t;
}

bool compare(int a, int b, int* compares) {
    fprintf(stderr, "comparing %d, with %d\n", a, b);
    (*compares)++;
    return a <= b;
}

int partition(int array[], int l, int h, int* compares, int* moves) {
    fprintf(stderr, "select_pivot = %d\n", array[l]);
    int i = l;

    for(int j = l + 1; j <= h; j++) {
        if(compare(array[j], array[l], compares)) {
            i++;
            swap(&array[i], &array[j], moves);
        }
    }
    swap(&array[i], &array[l], moves);
    return i;
}

int randomizedPartition(int *a, int l, int h, int* compares, int* moves) {
    pcg32_random_t rng;
    pcg32_srandom_r(&rng, time(NULL) ^ (intptr_t)&printf, 33u);
    int i = pcg32_random_r(&rng) % (h - l + 1);
    swap(&a[l + i], &a[l], moves);
    return partition(a, l, h, compares, moves);
}

int randomizedSelect(int *a, int p, int q, int i, int* compares, int* moves) {
    fprintf(stderr, "randomizedSelect------\n");
    list_printing(a, p, q);
    fprintf(stderr, "k = %d\n", i);

    if(p == q)
        return a[p];

    int r = randomizedPartition(a, p, q, compares, moves);
    int k = r - p + 1;

    if(i == k)
        return a[r];
    else if(i < k)
        return randomizedSelect(a, p, r - 1, i, compares, moves);
    else
        return randomizedSelect(a, r + 1, q, i - k, compares, moves);
}

void insertionSort(int *list,int l, int h, int *compares, int *moves) {
    int key;
    int i;

    for (int j = 1+l; j < h; j++) {
        key = list[j];
        fprintf(stderr, "Found key: %d\n", key);
        i = j - 1;
        while (i > l-1 && compare(key, list[i], compares)) {
            fprintf(stderr, "Changing %d to %d\n", list[i + 1], list[i]);
            list[i + 1] = list[i];
            (*moves)++;
            i--;
        }
        fprintf(stderr, "Changing %d to %d\n", list[i + 1], key);
        list[i + 1] = key;
        (*moves)++;
    }
}

int partitioning5(int *array, int l, int h, int* compares, int* moves) {
    if(h == l)
        return l;
    insertionSort(array,l,h,compares,moves);
    return l + (h-l)/2;
}

int select_pivot(int *a, int l, int h, int* compares, int* moves) {
    if(h - l < 5) {
        partitioning5(a, l, h, compares, moves);
        return a[l - (l-h)/2];
    }

    int n = h-l+1;
    int n1 = (int) (n/5);
    int medianArray[n1];

    for(int i = 0; i < n1; i++) {
        int left = 5*i;
        int subRight = left + 4;
        if(subRight > h)
            subRight = h;
        int median5 = partitioning5(a, l + left, l + subRight, compares, moves);
        medianArray[i] = a[median5];
    }
    return select_pivot(medianArray, 0, n1 - 1, compares, moves);
}

int preselectedPivotPartition(int a[], int l, int h, int pivotvalue, int* compares, int* moves) {
    int i;
    for(i = l; i <= h; i++) {
        if(a[i] == pivotvalue)
            break;
    }
    swap(&a[i], &a[l], moves);
    return partition(a, l, h, compares, moves);
}

int mySelect(int *a, int l, int h, int k, int* compares, int* moves) {
    fprintf(stderr, "Select started\n");
    list_printing(a, l, h);
    fprintf(stderr, "k = %d\n", k);

    if(l == h)
        return a[l];

    int pivot_v = select_pivot(a, l, h, compares, moves);
    int pivotIndex = preselectedPivotPartition(a, l, h, pivot_v, compares, moves);

    int i = pivotIndex-l+1;
    if(k == i)
        return pivot_v;
    else if(k < i)
        return mySelect(a, l, pivotIndex - 1, k, compares, moves);
    else
        return mySelect(a, pivotIndex + 1, h, k - i, compares, moves);
}


void randomArray(int *array, int n) {
    pcg32_random_t rng;
    pcg32_srandom_r(&rng, time(NULL) ^ (intptr_t)&printf, 15u);

    for(int i = 0; i < n; i++)
        array[i] = pcg32_random_r(&rng) % 100000;
}

void permutation(int *array, int n) {
    for(int i = 0; i < n; i++) {
        array[i] = i + 1;
    }
    pcg32_random_t rng;
    pcg32_srandom_r(&rng, time(NULL) ^ (intptr_t)&printf, 34u);
    for(int i = n - 1; i > 0; i--) {
        int j = pcg32_random_r(&rng) % (i + 1);
        int tmp = array[j];
        array[j] = array[i];
        array[i] = tmp;
    }
}



int main(int argc, char *argv[]) {
    int type = 2, n, k, scompares = 0, smoves = 0, rscompares = 0, rsmoves = 0;

    if (argc > 1){
        if (!strcmp(argv[1], "-r"))
            type = 1;
        else
            type = 2;
    }
    printf("Type n=");
    scanf("%d", &n);
//    n = 12;
    int* array = malloc(sizeof(int)*n);
    printf("Type k=");
//    k=7;
    scanf("%d", &k);

    if(type == 1)
        randomArray(array, n);
    else
        permutation(array, n);

    int* s = malloc(sizeof(int)*n);
    int* rs = malloc(sizeof(int)*n);

    for(int i = 0; i < n; i++) {
        rs[i] = s[i] = array[i];
    }
    int sort_v = mySelect(s, 0, n - 1, k, &scompares, &smoves);
    fprintf(stderr, "End of Select\n\n");

    int rs_v = randomizedSelect(rs, 0, n - 1, k, &rscompares, &rsmoves);
    fprintf(stderr, "End of Randomized Select\n");

    if(sort_v != rs_v) {
        fprintf(stderr, "\nAlgorithm do not work\n");
        exit(-1);
    }


    fprintf(stderr, "\nSelect: comparisons = %d moves = %d\n", scompares, smoves);
    fprintf(stderr, "Random Select comparisons = %d moves = %d\n\n", rscompares, rsmoves);

    for(int i = 0; i < n; i++) {
        if(rs[i] == rs_v)
            printf("[%d] ", rs[i]);
        else
            printf("%d ", rs[i]);
    }
    printf("\n");
    for(int i = 0; i < n; i++) {
        if(s[i] == sort_v)
            printf("[%d] ", s[i]);
        else
            printf("%d ", s[i]);
    }
    printf("\n");
    return 0;
}