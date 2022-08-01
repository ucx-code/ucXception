#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
int main()
{
    int n, m;
    srand (time(NULL));
    int times = 20 + 1;
    int i;
    for (i = 0;i < times;i++){
        n = 500;
        m = 64000;
        printf("%d %d\n", n, m);
        int a;
        for (a = 0;a < m;a++){
            int k = rand() % n + 1;
            int k2 = rand() % n + 1;
            printf("%d %d\n", k, k2);
        }
    }
}
