#include <stdio.h>
short matrix[35][35];
short n, best;

void findPoint(short  pc, short  printers, short  vector[35])
{
    short sum = 0, i;
    if (printers >= best) 
        return;

    for (i = 0;i < n;i++){
        if (matrix[pc][i] == 1) 
            vector[i]++;
    }
    vector[pc]++;
    for (i = 0;i < n;i++){
        if (vector[i] > 0) 
            sum++;
    }
    if (sum == n) {
        best = printers;
        for (i = 0;i < n;i++){
            if (matrix[pc][i] == 1) 
                vector[i]--;
        }
        vector[pc]--;
        return;
    }
    for (i = pc + 1;i < n;i++)
        findPoint(i, printers + 1, vector);
    for (i = 0;i < n;i++){
        if (matrix[pc][i] == 1) 
            vector[i]--;
    }
    vector[pc]--;
}

int main()
{
    short x, y, i;
    short vector[35];
    scanf("%hd", &n);
    best = n;
    for (i = 0;i < n;i++){
        vector[i] = 0;
    }
    while (scanf("%hd %hd", &x, &y) != EOF){
        matrix[x - 1][y - 1] = 1;
        matrix[y - 1][x - 1] = 1;
    }
    for (i = 0;i < n;i++)
        findPoint(i, 1, vector);
    printf("%hd\n", best);
    return 0;
}
