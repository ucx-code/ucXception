#include <stdio.h>
#include <stdlib.h>
int time[15000000 * 2];

int main()
{
    int n;
    scanf("%d\n", &n);
    int i;
    int maximoEnd = 0;
    for (i = 0;i < n;i++){
        register int a;
        int begin, end;
        scanf("%d %d", &begin, &end);
        if (end > maximoEnd) 
            maximoEnd = end;

        for (a = begin;a <= end;a++)
            time[a]++;
    }
    int maior = 0;
    for (i = 0;i <= maximoEnd;i++)
        if (time[i] > maior) 
            maior = time[i];
    printf("%d\n", maior);
    return 0;
}
