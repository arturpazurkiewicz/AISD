#include <stdio.h>
#include <time.h>
#include "pcg_basic.h"
#include <stdlib.h>

int binarySearch(int A[], int start,int end,int element,int* compares,int *calls){
    (*compares)++;
    (*calls)++;
    int r;
    if (start > end)
        return 0;
    int mid = (start+end) /2;
    (*compares)++;
    if (A[mid]== element) {
        return 1;
    }
    else if (element < A[mid])
        r = binarySearch(A,start,mid-1,element,compares,calls);
    else
        r = binarySearch(A,mid+1,end,element,compares,calls);
    (*compares)++;
    return r;
}

int partition(int* arr, int low, int high, int* lp);

void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void DualPivotQuickSort(int* arr, int low, int high)
{
    if (low < high) {
        int lp, rp;
        rp = partition(arr, low, high, &lp);
        DualPivotQuickSort(arr, low, lp - 1);
        DualPivotQuickSort(arr, lp + 1, rp - 1);
        DualPivotQuickSort(arr, rp + 1, high);
    }
}

int partition(int* arr, int low, int high, int* lp)
{
    if (arr[low] > arr[high])
        swap(&arr[low], &arr[high]);
    int j = low + 1;
    int g = high - 1, k = low + 1, p = arr[low], q = arr[high];
    while (k <= g) {
        if (arr[k] < p) {
            swap(&arr[k], &arr[j]);
            j++;
        }
        else if (arr[k] >= q) {
            while (arr[g] > q && k < g)
                g--;
            swap(&arr[k], &arr[g]);
            g--;
            if (arr[k] < p) {
                swap(&arr[k], &arr[j]);
                j++;
            }
        }
        k++;
    }
    j--;
    g++;
    swap(&arr[low], &arr[j]);
    swap(&arr[high], &arr[g]);
    *lp = j;

    return g;
}

int main(int argc, char *argv[]) {
    if (argc==1) {
        int seed = 75u;
        pcg32_random_t rng;
        pcg32_srandom_r(&rng, time(NULL) ^ (intptr_t) &printf, (intptr_t) &seed);
        FILE *file = fopen("dane.csv", "w");
        int z = 10;
        for (int i = 1000; i < 100001; i += 1000) {
            long time = 0;
            int *calls = malloc(sizeof(int));
            (*calls) = 0;
            int *compares = malloc(sizeof(int));
            (*compares) = 0;
            for (int j = 0; j < z; ++j) {
                int *list = malloc(sizeof(int) * i);
                for (int k = 0; k < i; ++k) {
                    list[i] = pcg32_random_r(&rng) % 1000000;
                }
                DualPivotQuickSort(list, 0, i);
                clock_t start = clock();
                for (int l = 0; l < 10000; ++l) {
                    binarySearch(list, 0, i, INT32_MIN, compares, calls);
                }
                clock_t end = clock();
                time += end - start;
            }
            fprintf(file, "%d;%d;%d;%lf\n", i, *compares / (z * 10000), *calls / (z * 10000),
                    ((double) time) / (z * 10000));
        }
        fclose(file);
    } else{
        int n = atoi(argv[1]);
        int *list = malloc(sizeof(int )*n);
        printf("Type data\n");
        for (int i = 0; i < n; ++i) {
            scanf("%d",&list[i]);
        }
        printf("Searched=");
        int s =0,compares=0,calls=0;
        scanf("%d",&s);
        int res = 0;
        res = binarySearch(list,0,n,s,&compares,&calls);
        printf("Result = %d, compares = %d, calls = %d\n",res,compares,calls);
    }
    return 0;
}
