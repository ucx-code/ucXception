#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
unsigned int begin[150000];
unsigned int end[150000];

void quick_sort(unsigned int* a, int n)
{
    if (n < 2) 
        return;

    register unsigned int p = a[n / 2];
    unsigned int* l = a;
    register unsigned int* r = a + n - 1;
    while (l <= r){
        if (*l < p) {
            l++;
            continue;
        }
        if (*r > p) {
            r--;
            continue;
        }
        register int t = *l;
        *l++ = *r;
        *r-- = t;
    }
    quick_sort(a, r - a + 1);
    quick_sort(l, a + n - l);
}

struct stat statbuf;
char* src;
unsigned int n;

void readM()
{
    register char* in = src;
    register int i = 0;
    register unsigned int* out = &n;
    while (*in > 47){
        *out = *out * 10 + *in - 48;
        ++in;
    }
    ++in;
    while (*in){
        out = &begin[i];
        while (*in > 47){
            *out = *out * 10 + *in - 48;
            ++in;
        }
        ++in;
        out = &end[i];
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
    quick_sort(&begin[0], n);
    quick_sort(&end[0], n);
    unsigned int conta = 0;
    unsigned int a = 0;
    for (i = 0;i < n;i++){
        if (begin[i] < end[a]) 
            ++conta;
        else
            ++a;
    }
    printf("%d\n", conta);
    return 0;
}
