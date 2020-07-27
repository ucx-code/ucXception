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
    int begin = 0;
    int linhas = n;
    int nextBegin = -1;
    int final = n;
    do{
        maximo = 0;
        int master = begin;
        for (i = master + 1;i < n;i++){
            if ((slot[i].begin != -1) && (slot[master].end <= slot[i].begin)) {
                slot[master].begin = -1;
                slot[master].end = -1;
                ++maximo;
                master = i;
            }else
                if ((slot[i].begin != -1) && (nextBegin == -1)) 
                    nextBegin = 1;

            if (slot[i].begin != -1) 
                final = i;
        }
        if ((slot[master].begin != -1) && (master == final)) {
            slot[master].begin = slot[master].end = -1;
            ++maximo;
        }
        begin = nextBegin;
        if (maximo != 0) {
            linhas -= maximo;
            ++linhas;
        }else
            break;
    } while (maximo != 0);
    printf("%d\n", linhas);
    return 0;
}
