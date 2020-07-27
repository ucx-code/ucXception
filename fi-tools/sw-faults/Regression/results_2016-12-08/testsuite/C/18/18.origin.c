
/* Projeto LPA Meta 1 2012/2013
 * Celso Mendes
 * Joao Pedro
 * Joao Magalhaes */

#include <stdio.h>
#include <math.h>
#include <limits.h>

int distancias[13][15];
float supermarkets[13][2];
int maxCap;
int solution[13];
unsigned int best = INT_MAX;
int n;
int count;

/* verifica se e solucao */
int isSolution()
{
    int i;
    for(i = 0; i <= n; i++)
    {
        if (solution[i] == 0)
            return 0;
    }
    return 1;
}

void shortestPath(int v, int len, int cap, int bound)
{
    int i;
    int aux = len + distancias[v][0]; 
    cap -= distancias[v][13]; /* recalculo capacidade atual */
    if((aux + (bound - distancias[v][14]) / 2) >= best) /* aplicar bound */
        return;
    
    if(isSolution() && aux < best) /* caso aceitacao */
    {
        best = aux;
        return;
    }
    
    if(v != 0) /* voltar ao deposito */
        shortestPath(0, len + distancias[v][0], maxCap, bound);
    
    
    for(i = 1; i <= n; i++)
    {
        if(solution[i] == 0)
        {
            if(cap >= distancias[i][13])
            {
                solution[i] = 1;
                shortestPath(i, len + distancias[v][i], cap, bound - distancias[i][14]);
                solution[i] = 0;
            }
        }
    }
    
}

int arred(double x)
{
    return floor(x + 0.5);
}

int main(int argc, const char * argv[])
{
    int i, j, bound = 0;
    unsigned int x, y;
    scanf("%d", &n);
    
    for(i = 1; i <= n; i++)
        scanf("%f %f %d", &supermarkets[i][0], &supermarkets[i][1], &distancias[i][13]);
    scanf("%f %f %d", &supermarkets[0][0], &supermarkets[0][1], &distancias[i][13]);
    scanf("%d", &maxCap);
    
    for(i = 0; i < n; i++)
    {
        for(j = i + 1; j <= n; j++)
        {
            distancias[i][j] = arred(sqrt(pow((supermarkets[i][0] - supermarkets[j][0]),2) + pow((supermarkets[i][1] - supermarkets[j][1]),2)));
            distancias[j][i] = distancias[i][j];
        }
    }
    
    /* calculo do bound - 2 arestas mais curtas */
    for(i = 1; i <= n; i++)
    {
        x = INT_MAX - 1;
        y = INT_MAX;
        
        for(j = 1; j <= n; j++)
        {
            if(i != j)
            {
                if(x > distancias[i][j])
                {
                    if(x < y) y = x;
                    x = distancias[i][j];
                }else if(y > distancias[i][j])
                {
                    if(y < x) x = y;
                    y = distancias[i][j];
                }
            }
        }
        distancias[i][14] = x + y;
        bound += distancias[i][14];
    }
    
    solution[0] = 1;
    shortestPath(0, 0, maxCap, bound);
    
    printf("%d\n", best);
    return 0;
}
