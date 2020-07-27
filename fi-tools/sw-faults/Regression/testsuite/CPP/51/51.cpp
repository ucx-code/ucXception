#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
struct _slot{
    int begin;
    int end;
};
struct _slot slot[150000];

bool operator <(const struct _slot& a, const struct _slot& b)
{
    return a.begin < b.begin;
}

int main()
{
    int n;
    scanf("%d\n", &n);
    int i;
    for (i = 0;i < n;i++){
        scanf("%d %d", &slot[i].begin, &slot[i].end);
    }
    std::sort(slot, slot + n);
    int maior = 1;
    int a;
    for (i = 0;i <= n - maior;i++){
        for (a = i + 1;a < n;a++){
            if (slot[a].begin > slot[i].end) 
                break;
        }
        if (a - i - 1 > maior) 
            maior = a - i - 1;
    }
    printf("%d\n", maior);
    return 0;
}
