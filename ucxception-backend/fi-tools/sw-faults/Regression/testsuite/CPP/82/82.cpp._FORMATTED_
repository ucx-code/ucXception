#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int friendship[2000];
int choices[2000][3];

void showC(int* c, int end)
{
    int i;
    for (i = 0;i < end;i++)
        printf("%d", c[i]);
    printf("\n");
}

int n, m;
int best = 9999999;
int last = best;

int calc(int* c)
{
    int i, a;
    int soma = 0;
    for (i = 0;i < m;i++){
        bool favoravel = false;
        for (a = 0;a < 3;a++){
            int absoluto = abs(choices[i][a]) - 1;
            if (choices[i][a] == 0) 
                break;

            if ((choices[i][a] > 0) && (c[absoluto] == 1)) {
                favoravel = true;
                break;
            }
            if ((choices[i][a] < 0) && (c[absoluto] == 0)) {
                favoravel = true;
                break;
            }
        }
        if (favoravel == false) {
            soma += friendship[i];
        }
    }
    return soma;
}
void rec(int* c, int begin, int end)
{
    int soma = calc(c);
    if (soma < best) {
        last = best;
        best = soma;
    }
    int i;
    for (i = begin + 1;i < end;i++){
        c[i] = 1;
        rec(c, i, end);
        c[i] = 0;
    }
}

int c[200];

int main()
{
    int i;
    scanf("%d %d", &n, &m);
    for (i = 0;i < m;i++){
        scanf("%d", &friendship[i]);
        int tmp;
        scanf("%d", &tmp);
        int a = 0;
        while (tmp != 0){
            choices[i][a++] = tmp;
            scanf("%d", &tmp);
        }
    }
    rec(c, -1, n);
    printf("%d\n", best);
}
