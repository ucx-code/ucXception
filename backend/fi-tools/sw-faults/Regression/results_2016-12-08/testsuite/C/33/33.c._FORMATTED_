#include <stdio.h>
int tab[35][35];
int n, best;
void addPrint(int pos, int nPrint, int* print);

int main()
{
    int x, y;
    int i;
    int print[35] = {0};
    scanf("%d", &n);
    best = n;
    while (scanf("%d %d", &x, &y) != EOF){
        tab[x - 1][y - 1] = 1;
        tab[y - 1][x - 1] = 1;
    }
    for (i = 0;i < n;i++){
        addPrint(i, 1, print);
    }
    printf("%d\n", best);
    return 0;
}

void addPrint(int pos, int nPrint, int* print)
{
    int i;
    int sum = 0;
    if (nPrint >= best) 
        return;

    print[pos]++;
    for (i = 0;i < n;i++){
        if (tab[pos][i] == 1) 
            print[i]++;
    }
    for (i = 0;i < n;i++){
        if (print[i] == 0) 
            sum++;
    }
    if (sum == 0) {
        best = nPrint;
        print[pos]--;
        for (i = 0;i < n;i++){
            if (tab[pos][i] == 1) 
                print[i]--;
        }
        return;
    }
    for (i = pos + 1;i < n;i++){
        addPrint(i, nPrint + 1, print);
    }
    print[pos]--;
    for (i = 0;i < n;i++){
        if (tab[pos][i] == 1) 
            print[i]--;
    }
}
