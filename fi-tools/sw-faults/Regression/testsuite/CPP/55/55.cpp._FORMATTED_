#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
struct _slot{
    int begin;
    int end;
};
struct _slot slot[150000];
struct _slotLL{
    int begin;
    int end;
    struct _slotLL* next;
    struct _slotLL* parent;
};

struct _slotLL* newSlot(int begin, int end)
{
    struct _slotLL* novo = (struct _slotLL*)malloc(sizeof (struct _slotLL));
    novo->begin = begin;
    novo->end = end;
    novo->next = novo->parent = NULL;
    return novo;
}

void appendSlot(struct _slotLL* p, struct _slotLL* n)
{
    struct _slotLL* tmp = p->next;
    n->next = tmp;
    if (p->next != NULL) 
        p->next->parent = n;

    p->next = n;
}

void removeSlot(struct _slotLL* parent, struct _slotLL* toRemove)
{
    if (parent != NULL) 
        parent->next = toRemove->next;
}
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
    struct _slotLL* head = newSlot(slot[0].begin, slot[0].end);
    for (i = n;i > 0;i--){
        struct _slotLL* tmp = newSlot(slot[i].begin, slot[i].end);
        appendSlot(head, tmp);
    }
    if (head->next != NULL) 
        head->next->parent = head;

    struct _slotLL* master = head;
    struct _slotLL* actual = master->next;
    int conta;
    int total = n;
    int t1 = n;
    do{
        master = head;
        actual = master->next;
        conta = 0;
        while (actual != NULL){
            printf("Master %d %d Actual %d %d\n", master->begin, master->end, actual->begin, actual->end);
            if (master->end <= actual->begin) {
                if (master == head) 
                    head = head->next;

                if (master->parent != NULL) 
                    master->parent->next = master->next;

                master = actual;
                ++conta;
                --t1;
            }
            actual = actual->next;
        }
        if (master != head) {
            if (master->parent != NULL) 
                master->parent->next = master->next;

            ++conta;
            --t1;
        }
        if (t1 < 0) {
            return 0;
        }
        if (conta != 0) 
            total -= conta - 1;
    } while (conta != 0);
    printf("%d\n", total);
    return 0;
}
