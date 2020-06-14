#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

void swap(int *x, int *y, int *moves) {
    ++(*moves);
    fprintf(stderr, "Swap: %d with %d\n", *x, *y);
    int temp = *x;
    *x = *y;
    *y = temp;
}

bool comparePom(int x, int y, bool comp) {
    if (!comp)
        return x <= y;
    else
        return y <= x;
}

bool compare(int x, int y, bool comp, int *compares) {
    ++(*compares);
    fprintf(stderr, "Comparing %d with %d\n", x, y);
    if (!comp)
        return x <= y;
    else
        return y <= x;
}

bool isSorted(int *x, int n, bool comp) {
    for (int i = 0; i + 1 < n; i++)
        if (!comparePom(x[i], x[i + 1], comp))
            return false;
    return true;
}

void insertionSort(int *list, int n, int *compares, int *moves, bool comp) {
    int key;
    int i;

    for (int j = 1; j < n; j++) {
        key = list[j];
        fprintf(stderr, "Found key: %d\n", key);
        i = j - 1;
        while (i > -1 && compare( key,list[i], comp, compares)) {
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

void merge(int arr[], int l, int m, int r, int *compares, int *moves, bool comp) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
    int L[n1], R[n2];
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (compare(L[i], R[j], comp, compares)) {
            arr[k] = L[i];
            i++;
        } else {
            ++(*moves);
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[], int l, int r, int *compares, int *moves, bool comp) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m, compares, moves, comp);
        mergeSort(arr, m + 1, r, compares, moves, comp);
        merge(arr, l, m, r, compares, moves, comp);
    }
}

void quickSort(int *list, int start, int end, int *compares, int *moves, bool comp) {
    if (start < end) {
        int x = list[start];
        int p = start;

        for (int j = start + 1; j <= end; j++) {
            if (compare(list[j], x, comp, compares)) {
                swap(&list[++p], &list[j], moves);
            }
        }
        if (list[p] != list[start]) {
            swap(&list[p], &list[start], moves);
        }

        quickSort(list, start, p - 1, compares, moves, comp);
        quickSort(list, p + 1, end, compares, moves, comp);
    }
}


int main(int argc, char *argv[]) {
    int type = 0;
    bool comp = false;

    argc--;
    argv++;
    while (argc > 0) {
        if (!strcmp((argv[0]), "--type")) {
            argc--;
            argv++;
            if (argc > 0) {
                if (!strcmp(argv[0], "insert"))
                    type = 1;
                else if (!strcmp(argv[0], "merge"))
                    type = 2;
                else if (!strcmp(argv[0], "quick"))
                    type = 3;
                argc--;
                argv++;
            }
        } else if (!strcmp((argv[0]), "--comp")) {
            argc--;
            argv++;
            if (argc > 0) {
                if (!strcmp(argv[0], "<="))
                    comp = false;
                else if (!strcmp(argv[0], ">="))
                    comp = true;
                argc--;
                argv++;
            }
        }
    }

    int n;
    scanf("%d", &n);

    int *list = malloc(sizeof(int) * n);


    for (int i = 0; i < n; i++) {
        scanf("%d", &(list[i]));
    }


    int compares = 0, counter = 0;
    double time;

    clock_t start = clock();
    switch (type) {
        case 1:
            insertionSort(list, n, &compares, &counter, comp);
            break;
        case 2:
            mergeSort(list, 0, n - 1, &compares, &counter, comp);
            break;
        default:
            quickSort(list, 0, n - 1, &compares, &counter, comp);
            break;
    }
    clock_t end = clock();

    time = (double) ((end - start) / 1000.0);
    if (!isSorted(list, n, comp)) {
        for (int i = 0; i < n - 1; i++)
            printf("%d ", list[i]);
        return 1;
    }
    fprintf(stderr, "Compares: %d , Moves: %d ,Time(ms): %f \n", compares, counter, time);

    printf("%d\n", n);
    for (int i = 0; i < n - 1; i++)
        printf("%d ", list[i]);
    printf("%d\n", list[n - 1]);
    return 0;
}

