#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <list>
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
    std::list<struct _slot*> lista;
    for (i = 0;i < n;i++){
        lista.push_back(&slot[i]);
    }
    int total = n;
    int rooms = 0;
    while (true){
        ++rooms;
        std::list<struct _slot*>::iterator ia = lista.begin();
        std::list<struct _slot*>::iterator master = lista.begin();
        for (;ia != lista.end();ia++){
            if ((*master)->end <= (*ia)->begin) {
                lista.erase(master);
                master = ia;
            }
        }
        if (master == lista.begin()) {
            lista.erase(master);
            break;
        }else{
            lista.erase(master);
        }
    }
    printf("%d\n", rooms + lista.size());
    return 0;
}
