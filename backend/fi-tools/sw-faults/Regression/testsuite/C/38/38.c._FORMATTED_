#include <stdio.h>
#include <stdlib.h>
int begin[150000], endTime[150000];

int sorting(const void* a, const void* b)
{
    return (*(int*)a - *(int*)b);
}

int main()
{
    int n, k = 1, j = 0, i;
    scanf("%d", &n);
    for (i = 0;i < n;i++){
        scanf("%d %d", &begin[i], &endTime[i]);
    }
    qsort(begin, n, sizeof (int), sorting);
    qsort(endTime, n, sizeof (int), sorting);
    for (i = 1;i < n;i++){
        if (endTime[j] > begin[i]) {
            k++;
        }else{
            j++;
        }
    }
    printf("%d\n", k);
    return 0;
}
