#include <stdio.h>
#include <math.h>
short distancias[13][13];
float supermarkets[13][2];
short solution[13];
unsigned short best = 65535;
short n;

int isSolution()
{
    short i;
    for (i = 0;i <= n;i++){
        if (solution[i] == 0) 
            return 0;
    }
    return 1;
}

void shortestPath(short  v, short  len)
{
    short i;
    short aux = len + distancias[v][0];
    if (aux >= best) 
        return;

    if (isSolution() && aux < best) {
        best = aux;
        return;
    }
    for (i = 1;i <= n;i++){
        if (solution[i] == 0) {
            solution[i] = 1;
            shortestPath(i, len + distancias[v][i]);
            solution[i] = 0;
        }
    }
}

int arred(double x)
{
    return floor(x + 0.5);
}

int main(int argc, const char* argv[])
{
    short i, j;
    scanf("%hd", &n);
    for (i = 1;i <= n;i++)
        scanf("%f %f", &supermarkets[i][0], &supermarkets[i][1]);
    scanf("%f %f", &supermarkets[0][0], &supermarkets[0][1]);
    for (i = 0;i < n;i++){
        for (j = i + 1;j <= n;j++){
            distancias[i][j] = arred(sqrt(pow((supermarkets[i][0] - supermarkets[j][0]), 2) + pow((supermarkets[i][1] - supermarkets[j][1]), 2)));
            distancias[j][i] = distancias[i][j];
        }
    }
    solution[0] = 1;
    shortestPath(0, 0);
    printf("%hd\n", best);
    return 0;
}
