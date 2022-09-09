#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
int vizinhos[35][34];
int counter[35];
int melhor = 9999;
int estadosP[35][35];
int n;

int isNeig(int i, int a)
{
    int z;
    for (z = 0;z < counter[z];z++){
        if (vizinhos[i][z] == a) 
            return 1;
    }
    return 0;
}

int rec(int pos, int n, int count, int estadoCounter)
{
    ++count;
    if (count > n / 2) 
        return -1;

    if (count >= melhor) 
        return -1;

    estadosP[estadoCounter][pos] = 2;
    int i;
    for (i = 0;i < counter[pos];i++){
        estadosP[estadoCounter][vizinhos[pos][i]] = 1;
    }
    int faltam = 0;
    std::vector<int> osQueFaltam;
    for (i = 0;i < n;i++){
        if (estadosP[estadoCounter][i] == 0) {
            ++faltam;
            osQueFaltam.push_back(i);
        }
    }
    if (faltam != 0) {
        int a, z;
        bool aindaDaMais = false;
        for (i = 0;i < osQueFaltam.size();i++){
            for (a = i + 1;a < osQueFaltam.size();a++){
                if (isNeig(i, a) == 1) {
                    aindaDaMais = true;
                    goto salta;
                }
            }
        }
        salta:
        if (aindaDaMais == false) {
            int final = count + faltam;
            if (count < melhor) 
                melhor = count;

            return 1;
        }
        bool todosTemAcesso = true;
        for (i = 0;i < n;i++){
            if (estadosP[estadoCounter][i] == 0) {
                todosTemAcesso = false;
                memcpy(estadosP[estadoCounter + 1], estadosP[estadoCounter], sizeof (int) * 35);
                rec(i, n, count, estadoCounter + 1);
            }
        }
        if ((todosTemAcesso)&&(count < melhor)) 
            melhor = count;
    }
    return 1;
}

int compare(const void* a, const void* b)
{
    return (counter[*(int*)b] + counter[*(int*)a]);
}

int main()
{
    scanf("%d", &n);
    if (n <= 1) {
        printf("%d\n", n);
        return 0;
    }
    int tmpA, tmpB;
    while (scanf("%d %d", &tmpA, &tmpB) != EOF){
        vizinhos[tmpA - 1][counter[tmpA - 1]] = tmpB - 1;
        vizinhos[tmpB - 1][counter[tmpB - 1]] = tmpA - 1;
        counter[tmpA - 1]++;
        counter[tmpB - 1]++;
    }
    int i;
    for (i = 0;i < n;i++){
        int a;
        for (a = 0;a < 35;a++)
            memset(estadosP[a], 0, sizeof (int) * 35);
        rec(i, n, 0, 0);
    }
    printf("%d\n", melhor);
    return 0;
}
