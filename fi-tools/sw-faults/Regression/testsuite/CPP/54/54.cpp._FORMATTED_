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
    return a.end < b.end;
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
    int a;
    int maximo = 0;
    int begin = 1;
    int linhas = n;
    maximo = 0;
    int master = 0;
    for (i = 1;i < n;i++){
        if ((slot[master].end <= slot[i].begin)) {
            ++maximo;
            master = i;
        }
    }
    printf("%d\n", n - maximo);
    return 0;
}
