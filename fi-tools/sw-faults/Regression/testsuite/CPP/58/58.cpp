#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <queue>
struct _slot{
    int begin;
    int end;
};
struct _slot slot[150000];
std::priority_queue<int,std::vector<int> ,std::greater<int> > room;

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
    int d = 1;
    room.push(slot[0].end);
    for (i = 1;i < n;i++){
        if (room.top() < slot[i].begin) {
            room.pop();
            room.push(slot[i].end);
        }else{
            room.push(slot[i].end);
            ++d;
        }
    }
    printf("%d\n", d);
    return 0;
}
