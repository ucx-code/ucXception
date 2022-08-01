#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <list>
struct _slot{
    int begin;
    int end;
};
struct _slot slot[150000];
int room[150000];

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
    int d = 0;
    for (i = 0;i < n;i++){
        int a;
        bool can = false;
        for (a = 0;a < d;a++)
            if (room[a] < slot[i].begin) {
                can = true;
                room[a] = slot[i].end;
                break;
            }
        if (can == false) {
            room[d++] = slot[i].end;
        }
    }
    printf("%d\n", d);
    return 0;
}
