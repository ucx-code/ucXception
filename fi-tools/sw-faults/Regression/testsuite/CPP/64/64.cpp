#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
struct _slot{
    int begin;
    int end;
};
struct _slot in[150000];
struct _slot slot[150000];

void shellSortPhase(int a[], int length, int gap)
{
    register int i;
    for (i = gap;i < length;++i){
        register int value = a[i];
        register int j;
        for (j = i - gap;j >= 0 && a[j] > value;j -= gap){
            a[j + gap] = a[j];
        }
        a[j + gap] = value;
    }
}

void shellSort(int a[], size_t length)
{
    static const int gaps[] = {1, 4, 10, 23, 57, 132, 301, 701};
    register int sizeIndex;
    for (sizeIndex = sizeof (gaps) / sizeof (gaps[0]) - 1;sizeIndex >= 0;--sizeIndex)
        shellSortPhase(a, length, gaps[sizeIndex]);
}

int N = 1;
int a[150000];

inline void upheap(int k)
{
    register int v;
    v = a[k];
    a[0] = -999999;
    while (a[k / 2] >= v){
        a[k] = a[k / 2];
        k = k / 2;
    }
    a[k] = v;
}
inline void downheap(int k)
{
    register int j, v;
    v = a[k];
    while (k <= N / 2){
        j = k + k;
        if (j < N && a[j] > a[j + 1]) 
            j++;

        if (v <= a[j]) 
            break;

        a[k] = a[j];
        k = j;
    }
    a[k] = v;
}

void insert(int v)
{
    a[++N] = v;
    upheap(N);
}

void replace(int v)
{
    a[0] = v;
    downheap(0);
}

int naa[150000];

int main()
{
    int n;
    scanf("%d\n", &n);
    int i;
    for (i = 0;i < n;i++){
        scanf("%d %d", &slot[i].begin, &slot[i].end);
        naa[i] = slot[i].begin;
    }
    shellSort(naa, n);
    return 0;
    a[1] = slot[0].end;
    for (i = 1;i < n;i++){
        if (a[1] <= slot[i].begin) {
            replace(slot[i].end);
        }else{
            insert(slot[i].end);
        }
    }
    printf("%d\n", N);
    return 0;
}
