#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/fcntl.h>
struct _slot{
    unsigned int begin;
    unsigned int end;
};
struct _slot slot[150001];

int compare(const void* a, const void* b)
{
    return (((struct _slot*)a)->begin - ((struct _slot*)b)->begin);
}

unsigned int N = 1;

inline void upheap(unsigned int k)
{
    register unsigned int v;
    v = slot[k].begin;
    slot[0].begin = 0;
    while (slot[k / 2].begin >= v){
        slot[k].begin = slot[k / 2].begin;
        k = k / 2;
    }
    slot[k].begin = v;
}

inline void downheap(unsigned int k)
{
    register unsigned int j, v;
    v = slot[k].begin;
    while (k <= N / 2){
        j = k + k;
        if (j < N && slot[j].begin > slot[j + 1].begin) 
            j++;

        if (v <= slot[j].begin) 
            break;

        slot[k].begin = slot[j].begin;
        k = j;
    }
    slot[k].begin = v;
}

void insert(int v)
{
    slot[++N].begin = v;
    upheap(N);
}

void replace(int v)
{
    slot[0].begin = v;
    downheap(0);
}

struct stat statbuf;
char* src;
unsigned int n;

void readM()
{
    register char* in = src;
    register int i = 1;
    register unsigned int* out = &n;
    while (*in > 47){
        *out = *out * 10 + *in - 48;
        ++in;
    }
    ++in;
    while (*in){
        out = &slot[i].begin;
        while (*in > 47){
            *out = *out * 10 + *in - 48;
            ++in;
        }
        ++in;
        out = &slot[i].end;
        while (*in > 47){
            *out = *out * 10 + *in - 48;
            ++in;
        }
        ++in;
        ++i;
    }
}

int main()
{
    unsigned int i;
    fstat(STDIN_FILENO, &statbuf);
    src = (char*)mmap(0, statbuf.st_size, PROT_READ, MAP_PRIVATE, 0, 0);
    madvise(src, statbuf.st_size, MADV_WILLNEED | MADV_SEQUENTIAL);
    readM();
    madvise(src, statbuf.st_size, MADV_DONTNEED);
    qsort(&slot[1], n, sizeof (struct _slot), compare);
    slot[1].begin = slot[1].end;
    for (i = 2;i <= n;i++){
        if (slot[1].begin <= slot[i].begin) {
            replace(slot[i].end);
        }else{
            insert(slot[i].end);
        }
    }
    printf("%d\n", N);
    return 0;
}
