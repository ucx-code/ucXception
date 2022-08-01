#include <stdio.h>
int tab[50][50];
int vec[50];
int n, m, k, nSlots;

int procrastination(int exam)
{
    int z, i;
    if (nSlots == 0 && n > exam) 
        return 0;

    if (n == exam) {
        return 1;
    }
    for (i = 1;i <= k;i++){
        for (z = 0;z < n;z++){
            if (vec[z] == i && tab[exam][z] == 1) {
                break;
            }
        }
        if (z == n) {
            vec[exam] = i;
            nSlots++;
            if (procrastination(exam + 1) == 1) {
                return 1;
            }
            nSlots--;
            vec[exam] = 0;
        }
    }
    return 0;
}

int main()
{
    int i, x, y;
    while (scanf("%d %d %d", &n, &m, &k) != EOF){
        for (i = 0;i < m;i++){
            scanf("%d %d", &x, &y);
            tab[x - 1][y - 1] = 1;
            tab[y - 1][x - 1] = 1;
        }
        nSlots = k;
        if (procrastination(-1) == 1) {
            printf("YES!\n");
        }else{
            printf("NO!\n");
        }
    }
    return 0;
}

