#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "pcg_basic.h"
#include <sys/sysinfo.h>


void swap(int *x, int *y, int *moves) {
    ++(*moves);
//    fprintf(stderr, "Swap: %d with %d\n", *x, *y);
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
//    fprintf(stderr, "Comparing %d with %d\n", x, y);
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
//        fprintf(stderr, "Found key: %d\n", key);
        i = j - 1;
        while (i > -1 && compare(key, list[i], comp, compares)) {
//            fprintf(stderr, "Changing %d to %d\n", list[i + 1], list[i]);
            list[i + 1] = list[i];
            (*moves)++;
            i--;
        }
//        fprintf(stderr, "Changing %d to %d\n", list[i + 1], key);
        list[i + 1] = key;
        (*moves)++;
    }
}

void radix_sort(int *a, int n, int *compares, int *moves, bool comp) {
    int i, b[n], m = a[0], exp = 1;
    for (i = 1; i < n; i++) {
        if (compare(m, a[i], comp, compares))
            m = a[i];
    }
    while (m / exp > 0) {
        int box[10] = {
                0
        };
        for (i = 0; i < n; i++)
            box[a[i] / exp % 10]++;
        for (i = 1; i < 10; i++)
            box[i] += box[i - 1];
        for (i = n - 1; i >= 0; i--)
            b[--box[a[i] / exp % 10]] = a[i];
        for (i = 0; i < n; i++) {
            a[i] = b[i];
            (*moves)++;
        }
        exp *= 10;
    }
}

void merge(int arr[], int l, int m, int r, int *compares, int *moves, bool comp) {
//    fprintf(stderr, "Merging\n");
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
        int *a, b;
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

void dualPivotQuickSort(int *A, int left, int right, int *compares, int *moves, bool comp) {
    if (right <= left) {
        return;
    }
    if (compare(A[right], A[left], comp, compares)) {
        swap(&A[left], &A[right], moves);
    }
    int p = A[left];
    int q = A[right];
    int i = left + 1;
    int k = right - 1;
    int j = i;
    int d = 0;
    while (j <= k) {
        if (d >= 0) {
            if (compare(A[j], p, comp, compares)) {
                swap(&A[i], &A[j], moves);
                i++;
                j++;
                d++;
            } else {
                if (compare(A[j], q, comp, compares)) {
                    j++;
                } else {
                    swap(&A[j], &A[k], moves);
                    k--;
                    d--;
                }
            }
        } else {
            if (!compare(A[k], q, comp, compares)) {
                k--;
                d--;
            } else {
                if (compare(A[k], p, comp, compares)) {
                    int tmp = A[k];
                    A[k] = A[j];
                    A[j] = A[i];
                    A[i] = tmp;
                    i++;
                    d++;
                    ++(*moves);
                } else {
                    swap(&A[j], &A[k], moves);
                }
                j++;
            }
        }
    }
    swap(&A[left], &A[i - 1], moves);
    swap(&A[right], &A[k + 1], moves);
    dualPivotQuickSort(A, left, i - 1, compares, moves, comp);
    dualPivotQuickSort(A, i, k, compares, moves, comp);
    dualPivotQuickSort(A, k + 2, right, compares, moves, comp);
}

void hybridSort(int *A, int left, int right, int *compares, int *moves, bool comp) {
    if (right <= left) {
        return;
    }
    if (right - left > 5) {
        if (compare(A[right], A[left], comp, compares)) {
            swap(&A[left], &A[right], moves);
        }
        int p = A[left];
        int q = A[right];
        int i = left + 1;
        int k = right - 1;
        int j = i;
        int d = 0;
        while (j <= k) {
            if (d >= 0) {
                if (compare(A[j], p, comp, compares)) {
                    swap(&A[i], &A[j], moves);
                    i++;
                    j++;
                    d++;
                } else {
                    if (compare(A[j], q, comp, compares)) {
                        j++;
                    } else {
                        swap(&A[j], &A[k], moves);
                        k--;
                        d--;
                    }
                }
            } else {
                if (!compare(A[k], q, comp, compares)) {
                    k--;
                    d--;
                } else {
                    if (compare(A[k], p, comp, compares)) {
                        int tmp = A[k];
                        A[k] = A[j];
                        A[j] = A[i];
                        A[i] = tmp;
                        i++;
                        d++;
                        ++(*moves);
                    } else {
                        swap(&A[j], &A[k], moves);
                    }
                    j++;
                }
            }
        }
        swap(&A[left], &A[i - 1], moves);
        swap(&A[right], &A[k + 1], moves);
        hybridSort(A, left, i - 1, compares, moves, comp);
        hybridSort(A, i, k, compares, moves, comp);
        hybridSort(A, k + 2, right, compares, moves, comp);
    } else {
        quickSort(A, left, right, compares, moves, comp);
    }
}

double sorter(int *list, int *compares, int *moves, bool comp, int type, int n) {
    double time;
    clock_t start = clock();
    switch (type) {
        case 1:
            insertionSort(list, n, compares, moves, comp);
            break;
        case 2:
            mergeSort(list, 0, n - 1, compares, moves, comp);
            break;
        case 3:
            quickSort(list, 0, n - 1, compares, moves, comp);
            break;
        case 4:
            dualPivotQuickSort(list, 0, n - 1, compares, moves, comp);
            break;
        case 6:
            radix_sort(list, n, compares, moves, false);
            break;
        default:
            hybridSort(list, 0, n - 1, compares, moves, comp);
            break;
    }
    clock_t end = clock();

    time = (double) ((double) (end - start) / 1000.0);

//    if (!isSorted(list, n, comp)) {
//        for (int i = 0; i < n - 1; i++)
//            printf("Do not work %d %d,%d\n", list[i], i, n);
//        exit(-1);
//    }
    //fprintf(stderr, "Compares: %d , Moves: %d ,Time(ms): %f \n", *compares, *moves, time);
    return time;
}

int main(int argc, char *argv[]) {
    struct sysinfo sys_info;
    if (sysinfo(&sys_info) != 0) perror("sysinfo");
    unsigned long long ram_start = (sys_info.totalram * (unsigned long long) sys_info.mem_unit / 1024) -
                                   (sys_info.freeram * (unsigned long long) sys_info.mem_unit / 1024);

    int type = 0;
    bool comp = false;
    char *fileName = "";
    bool toFile = false;
    int k = 1;
    argc--;
    argv++;
    int compares = 0;
    int moves = 0;
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
                else if (!strcmp(argv[0], "dpqs"))
                    type = 4;
                else if (!strcmp(argv[0], "hybrid"))
                    type = 5;
                else if (!strcmp(argv[0], "radix"))
                    type = 6;
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
        } else if (!strcmp((argv[0]), "--stat")) {
            argc--;
            argv++;
            if (argc > 0) {
                toFile = true;
                fileName = argv[0];
                argc--;
                argv++;
                if (argc > 0)
                    k = atoi(argv[0]);
            }
        } else {
            argc--;
            argv++;
        }
    }
    if (toFile) {
        int seed = 0;
        pcg32_random_t rng;
        pcg32_srandom_r(&rng, time(NULL) ^ (intptr_t) &printf, (intptr_t) &seed);
        FILE *file = fopen(fileName, "w");
        if (file == NULL) {
            fprintf(stderr, "Something went wrong");
            return 1;
        }
        if (type != 0) {
            fprintf(file, "n;k;compares;moves;time\n");
            bool five = true;
            for (int i = 10; i < 100001;) {

                struct sysinfo sys_info;
                if (sysinfo(&sys_info) != 0) perror("sysinfo");
                double t = 0;
                int c = 0;
                int m = 0;
                unsigned long long mem = 0;
                for (int j = 0; j < k; ++j) {
                    int *list = malloc(sizeof(int) * i);
                    for (int l = 0; l < i; ++l) {
                        list[l] = abs(pcg32_random_r(&rng));
                    }
                    t += sorter(list, &c, &m, comp, type, i);
                    unsigned long long pam = (sys_info.totalram * (unsigned long long) sys_info.mem_unit / 1024) -
                                             (sys_info.freeram * (unsigned long long) sys_info.mem_unit / 1024) -
                                             ram_start;
                    mem += pam;
                    free(list);
                }
                fprintf(file, " %d; %d; %f; %f; %lf; %f\n", i, k, (double) (1.0 * c / k), (double) (1.0 * m / k),
                        (t / k), sys_info.loads[0] * t * (unsigned long long) sys_info.mem_unit / (1024 * k));
                if (five){
                    five = false;
                    i*=5;
                } else{
                    five = true;
                    i*=2;
                }
            }
        } else {
            fprintf(file, "n;k;compares(i);moves(i);time(i);"
                          "compares(m);moves(m);time(m);"
                          "compares(q);moves(q);time(q);"
                          "compares(d);moves(d);time(d);"
                          "compares(h);moves(h);time(h);\n");
            for (int i = 100; i < 10001; i += 100) {
                double t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0;
                int c1 = 0, c2 = 0, c3 = 0, c4 = 0, c5 = 0;
                int m1 = 0, m2 = 0, m3 = 0, m4 = 0, m5 = 0;
                for (int j = 0; j < k; ++j) {
                    int *l1 = malloc(sizeof(int) * i);
                    int *l2 = malloc(sizeof(int) * i);
                    int *l3 = malloc(sizeof(int) * i);
                    int *l4 = malloc(sizeof(int) * i);
                    int *l5 = malloc(sizeof(int) * i);

                    for (int l = 0; l < i; ++l) {
                        l1[l] = l2[l] = l3[l] = l4[l] = l5[l] = abs(pcg32_random_r(&rng));
                    }
                    t1 += sorter(l1, &c1, &m1, comp, 1, i);
                    t2 += sorter(l2, &c2, &m2, comp, 2, i);
                    t3 += sorter(l3, &c3, &m3, comp, 3, i);
                    t4 += sorter(l4, &c4, &m4, comp, 4, i);
                    t5 += sorter(l5, &c5, &m5, comp, 5, i);

                }
                fprintf(file, " %d;%d;%f;%f;%lf;%f;%f;%lf;%f;%f;%lf;%f;%f;%lf;%f;%f;%lf\n", i, k,
                        (double) (1.0 * c1 / k), (double) (1.0 * m1 / k), (double) (1.0 * t1 / k),
                        (double) (1.0 * c2 / k), (double) (1.0 * m2 / k), (double) (1.0 * t2 / k),
                        (double) (1.0 * c3 / k), (double) (1.0 * m3 / k), (double) (1.0 * t3 / k),
                        (double) (1.0 * c4 / k), (double) (1.0 * m4 / k), (double) (1.0 * t4 / k),
                        (double) (1.0 * c5 / k), (double) (1.0 * m5 / k), (double) (1.0 * t5 / k));

            }
        }
    } else {
        int n;
        printf("Type n=");
        scanf("%d", &n);
        int *list = malloc(sizeof(int) * n);
        for (int i = 0; i < n; i++) {
            scanf("%d", &(list[i]));
        }
        double time = sorter(list, &compares, &moves, comp, type, n);
        for (int i = 0; i < n; i++)
            printf("%d ", list[i]);
        printf("Size: %d, ram: %f\n", n,
               sys_info.loads[0] * time * (unsigned long long) sys_info.mem_unit / (1024 * k));
    }
}

