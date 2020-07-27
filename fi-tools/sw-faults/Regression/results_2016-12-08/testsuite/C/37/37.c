#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
int compare(const void* a, const void* b)
{
    return (*(int*)a - *(int*)b);
}

int main()
{
    int fim[150000];
    int ini[150000];
    int i, j = 0;
    int n, totalROOMS = 1;
    scanf("%d", &n);
    for (i = 0;i < n;i++){
        scanf("%d %d", &ini[i], &fim[i]);
    }
    qsort(ini, n, sizeof (int), compare);
    qsort(fim, n, sizeof (int), compare);
    for (i = 1;i < n;i++){
        if (fim[j] > ini[i]) {
            totalROOMS++;
        }else
            j++;
    }
    printf("%d\n", totalROOMS);
    return 0;
}
