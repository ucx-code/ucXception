#include <stdio.h>
#include <stdlib.h>
struct _slot{
    int begin;
    int end;
};
struct _slot slot[150001];

int compare(const void* a, const void* b)
{
    return (((struct _slot*)a)->begin - ((struct _slot*)b)->begin);
}

int N = 1;

inline void upheap(int k)
{
    register int v;
    v = slot[k].begin;
    slot[0].begin = -999999;
    while (slot[k / 2].begin >= v){
        slot[k].begin = slot[k / 2].begin;
        k = k / 2;
    }
    slot[k].begin = v;
}

inline void downheap(int k)
{
    register int j, v;
    v = slot[k].begin;
    while (k <= N / 2){
        j = k + k;
        if (j < N && slot[j].begin > slot[j + 1].begin) 
            j++;

        if (v <= slot[j].begin) 
            break;

        slot[k].begin = slot[j].begin;
        k = j;
    }
    slot[k].begin = v;
}

void insert(int v)
{
    slot[++N].begin = v;
    upheap(N);
}

void replace(int v)
{
    slot[0].begin = v;
    downheap(0);
}

int main()
{
    int n;
    scanf("%d\n", &n);
    int i;
    for (i = 1;i <= n;i++){
        scanf("%d %d", &slot[i].begin, &slot[i].end);
    }
    qsort(&slot[1], n, sizeof (struct _slot), compare);
    slot[1].begin = slot[1].end;
    for (i = 2;i <= n;i++){
        if (slot[1].begin <= slot[i].begin) {
            replace(slot[i].end);
        }else{
            insert(slot[i].end);
        }
    }
    printf("%d\n", N);
    return 0;
}
