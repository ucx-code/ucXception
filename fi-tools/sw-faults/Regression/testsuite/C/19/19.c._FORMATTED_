#include <stdio.h>
#include <math.h>
#include <limits.h>
int distancias[17][19];
float supermarkets[17][2];
int lista[17][17];
int maxCap;
int solution[17];
unsigned int best = INT_MAX;
int n;
int count;

int isSolution()
{
    int i;
    for (i = 0;i <= n;i++){
        if (solution[i] == 0) 
            return 0;
    }
    return 1;
}

void shortestPath(int v, int len, int cap, int bound)
{
    int i;
    int aux = len + distancias[v][0];
    cap -= distancias[v][17];
    if (v != 0) {
        for (i = 1;i <= lista[v][0];i++){
            if (solution[lista[v][i]] == 0) 
                return;
        }
    }
    if ((aux + (bound - distancias[v][18]) / 2) >= best) 
        return;

    if (isSolution() && aux < best) {
        best = aux;
        return;
    }
    if (v != 0) 
        shortestPath(0, len + distancias[v][0], maxCap, bound);

    for (i = 1;i <= n;i++){
        if (solution[i] == 0) {
            if (cap >= distancias[i][17]) {
                solution[i] = 1;
                shortestPath(i, len + distancias[v][i], cap, bound - distancias[i][18]);
                solution[i] = 0;
            }
        }
    }
}

int arred(double x)
{
    return floor(x + 0.5);
}

int main(int argc, const char* argv[])
{
    int i, j, bound = 0;
    unsigned int x, y;
    scanf("%d", &n);
    for (i = 1;i <= n;i++)
        scanf("%f %f %d", &supermarkets[i][0], &supermarkets[i][1], &distancias[i][17]);
    scanf("%f %f %d", &supermarkets[0][0], &supermarkets[0][1], &distancias[i][17]);
    scanf("%d", &maxCap);
    while (scanf("%d %d", &x, &y) != EOF)
        lista[y][++lista[y][0]] = x;
    for (i = 0;i < n;i++){
        for (j = i + 1;j <= n;j++){
            distancias[i][j] = arred(sqrt(pow((supermarkets[i][0] - supermarkets[j][0]), 2) + pow((supermarkets[i][1] - supermarkets[j][1]), 2)));
            distancias[j][i] = distancias[i][j];
        }
    }
    for (i = 1;i <= n;i++){
        x = INT_MAX - 1;
        y = INT_MAX;
        for (j = 1;j <= n;j++){
            if (i != j) {
                if (x > distancias[i][j]) {
                    if (x < y) 
                        y = x;

                    x = distancias[i][j];
                }else
                    if (y > distancias[i][j]) {
                        if (y < x) 
                            x = y;

                        y = distancias[i][j];
                    }
            }
        }
        distancias[i][18] = x + y;
        bound += distancias[i][18];
    }
    solution[0] = 1;
    shortestPath(0, 0, maxCap, bound);
    printf("%d\n", best);
    return 0;
}
