#include <stdio.h>
#include <stdlib.h>
int begin[2000][4];
int fila[1001][2];

int sorting(const void* a, const void* b)
{
    return (*(int*)a - *(int*)b);
}

int main()
{
    int i = 0, j = 0, ant, k, area = 0, alt = 0, per = 0, alta, aux1, aux2, aux3, aux4;
    while (scanf("%d %d %d %d", &begin[i][0], &begin[i][1], &begin[i][2], &begin[i][3]) != EOF){
        i++;
        begin[i][0] = begin[i - 1][2];
        begin[i][1] = begin[i - 1][3];
        begin[i][2] = begin[i - 1][0];
        begin[i][3] = begin[i - 1][1];
        i++;
    }
    qsort(begin, i, 4 * sizeof (int), sorting);
    fila[0][0] = begin[0][1];
    fila[0][1] = begin[0][3];
    for (j = 1;j < i;j++){
        alta = alt;
        alt = fila[0][1] - fila[0][0];
        k = 1;
        ant = 0;
        while (fila[k][0] != 0){
            if (fila[k][0] < fila[ant][1] && fila[k][1] > fila[ant][1]) {
                alt += (fila[k][1] - fila[ant][1]);
                ant = k;
            }else
                if (fila[k][0] > fila[ant][1]) {
                    alt += (fila[k][1] - fila[k][0]);
                    ant = k;
                    per += 2 * (begin[j][0] - begin[j - 1][0]);
                }

            k++;
        }
        if (alta < alt) 
            per += (2 * (alt - alta));

        area = area + ((begin[j][0] - begin[j - 1][0]) * (alt));
        per += 2 * (begin[j][0] - begin[j - 1][0]);
        if (begin[j][0] < begin[j][2]) {
            k = 0;
            while (fila[k][0] != 0 && fila[k][0] < begin[j][1])
                k++;
            aux1 = fila[k][0];
            aux2 = fila[k][1];
            fila[k][0] = begin[j][1];
            fila[k][1] = begin[j][3];
            while (fila[++k][0] != 0){
                aux3 = fila[k][0];
                aux4 = fila[k][1];
                fila[k][0] = aux1;
                fila[k][1] = aux2;
                aux1 = aux3;
                aux2 = aux4;
            }
            fila[k][0] = aux1;
            fila[k][1] = aux2;
        }else{
            k = 0;
            while ((fila[k][1] != begin[j][1] && fila[k][0] != begin[j][3]) && fila[k][0] != 0)
                k++;
            while (fila[k][0] != 0){
                fila[k][0] = fila[k + 1][0];
                fila[k][1] = fila[k + 1][1];
                k++;
            }
        }
    }
    printf("%d %d\n", area, per);
    return 0;
}
