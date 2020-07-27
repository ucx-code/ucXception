#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int N, R;
char tab[1024][1025];
void expand(int comp);
void invert(int x, int y, int comp);

int main()
{
    while (scanf("%d ", &N) != EOF){
        int i;
        scanf("%d", &R);
        tab[0][0] = '0';
        tab[0][1] = '\0';
        if (N > 0) {
            R = R % 4;
            expand(1);
        }
        for (i = 0;i < pow(2, N);i++){
            printf("%s", tab[i]);
            printf("\n");
        }
    }
    return 0;
}

void expand(int comp)
{
    int i;
    if (R == 0) {
        for (i = 0;i < comp;i++){
            memcpy(&tab[i][comp], &tab[i][0], comp * sizeof (char));
            memcpy(&tab[i + comp][0], &tab[i][0], comp * sizeof (char));
        }
        invert(comp, comp, comp);
    }else
        if (R == 1) {
            for (i = 0;i < comp;i++){
                memcpy(&tab[i + comp][0], &tab[i][0], comp * sizeof (char));
                memcpy(&tab[i + comp][comp], &tab[i][0], comp * sizeof (char));
            }
            invert(0, comp, comp);
        }else
            if (R == 2) {
                for (i = 0;i < comp;i++){
                    memcpy(&tab[i][comp], &tab[i][0], comp * sizeof (char));
                    memcpy(&tab[i + comp][0], &tab[i][0], comp * sizeof (char));
                    memcpy(&tab[i + comp][comp], &tab[i][0], comp * sizeof (char));
                }
                invert(0, 0, comp);
            }else
                if (R == 3) {
                    for (i = 0;i < comp;i++){
                        memcpy(&tab[i][comp], &tab[i][0], comp * sizeof (char));
                        memcpy(&tab[i + comp][comp], &tab[i][0], comp * sizeof (char));
                    }
                    invert(comp, 0, comp);
                }

    if (comp * 2 == pow(2, N)) 
        return;

    expand(comp * 2);
}

void invert(int x, int y, int comp)
{
    int i, j;
    for (i = 0;i < comp;i++){
        for (j = 0;j < comp;j++){
            if (tab[i][j] == '0') 
                tab[x + i][y + j] = '1';
            else
                tab[x + i][y + j] = '0';
        }
        tab[i][comp + comp] = '\0';
        tab[i + comp][comp + comp] = '\0';
    }
}