#include <stdlib.h>
#include <stdio.h>
char tab[100][100];
int coord[100][2];
int nLinhas, nColunas;
int encont = 0;
int verifica(int l, int c, char prox, int ant[]);
void addCoord(int x, int y);
void removeCoord();

int main()
{
    while (scanf("%d ", &nLinhas) != EOF){
        encont = 0;
        int i, aux = 0, ant[2];
        scanf("%d", &nColunas);
        for (i = 0;i < nLinhas;i++)
            scanf("%s", tab[i]);
        coord[0][0] = coord[0][1] = -1;
        for (i = 0;i < nColunas;i++){
            ant[0] = 0;
            ant[1] = i;
            if (nLinhas == 1 && (tab[0][i] == '|' || tab[0][i] == '/' || tab[0][i] == '\\')) {
                encont = 1;
                addCoord(0, i);
                break;
            }
            if (nLinhas > 1) {
                if (tab[0][i] == '|') {
                    verifica(1, i - 1, '/', ant);
                    verifica(1, i, '|', ant);
                    verifica(1, i + 1, '\\', ant);
                }else
                    if (tab[0][i] == '/') {
                        verifica(1, i, '\\', ant);
                        verifica(1, i - 1, '/', ant);
                        verifica(1, i - 1, '|', ant);
                    }else
                        if (tab[0][i] == '\\') {
                            verifica(1, i, '/', ant);
                            verifica(1, i + 1, '\\', ant);
                            verifica(1, i + 1, '|', ant);
                        }

                if (encont == 1) {
                    addCoord(0, i);
                    break;
                }
            }
        }
        if (encont == 1) {
            for (i = 100;i >= 0;i--){
                if (aux == 1 && i != 0) 
                    printf("(%d,%d),", coord[i][0] + 1, coord[i][1] + 1);
                else
                    if (i == 0) 
                        printf("(%d,%d)\n", coord[i][0] + 1, coord[i][1] + 1);

                if (coord[i][0] == -1 && coord[i][1] == -1) 
                    aux = 1;
            }
        }else
            printf("No Path!\n");

        for (i = 0;i < 100;i++){
            coord[i][0] = coord[i][1] = 0;
        }
    }
    return 0;
}

int verifica(int l, int c, char prox, int* ant)
{
    int aux[2];
    if (l >= 0 && l < nLinhas && c >= 0 && c < nColunas && encont == 0) {
        if (prox == tab[l][c]) {
            aux[0] = l;
            aux[1] = c;
            if (l == nLinhas - 1) {
                encont = 1;
            }
            if (tab[l][c] == '|') {
                if (encont == 0) 
                    verifica(l + 1, c - 1, '/', aux);

                if (encont == 0) 
                    verifica(l + 1, c, '|', aux);

                if (encont == 0) 
                    verifica(l + 1, c + 1, '\\', aux);
            }else
                if (tab[l][c] == '/') {
                    if (encont == 0) 
                        verifica(l + 1, c, '\\', aux);

                    if (encont == 0) 
                        verifica(l + 1, c - 1, '/', aux);

                    if (encont == 0) 
                        verifica(l + 1, c - 1, '|', aux);

                    if (ant[1] != c + 1 && encont == 0) 
                        verifica(l, c + 1, '\\', aux);

                    if (ant[1] != c - 1 && encont == 0) 
                        verifica(l, c - 1, '\\', aux);
                }else
                    if (tab[l][c] == '\\') {
                        if (encont == 0) 
                            verifica(l + 1, c, '/', aux);

                        if (encont == 0) 
                            verifica(l + 1, c + 1, '\\', aux);

                        if (encont == 0) 
                            verifica(l + 1, c + 1, '|', aux);

                        if (ant[1] != c + 1 && encont == 0) 
                            verifica(l, c + 1, '/', aux);

                        if (ant[1] != c - 1 && encont == 0) 
                            verifica(l, c - 1, '/', aux);
                    }

            if (encont == 1) 
                addCoord(l, c);

            tab[l][c] = '#';
        }else{
            return 0;
        }
    }
    return 0;
}

void addCoord(int x, int y)
{
    int i;
    for (i = 0;i < 100;i++){
        if (coord[i][0] == -1 && coord[i][1] == -1) {
            coord[i][0] = x;
            coord[i][1] = y;
            coord[i + 1][0] = coord[i + 1][1] = -1;
            break;
        }
    }
}
