#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <algorithm>
int vizinhos[35][34];
int counter[35];
int sets[35][35];
int sizeSet[35];
int n;

void mostraVector(std::vector<int> x)
{
    int i;
    printf("(");
    for (i = 0;i < (int)x.size();i++)
        printf("%d ", x[i]);
    printf(")\n");
}

int melhor = 9999;

std::vector<int> MSC(std::vector<int> S, std::vector<int> U, int c)
{
    ++c;
    if (S.size() == 0) {
        if (U.size() == 0) {
            std::vector<int> empty;
            return empty;
        }else{
            std::vector<int> bad;
            bad.push_back(-1);
            return bad;
        }
    }
    int maior = -1;
    int maiorID = -1;
    int i;
    for (i = 0;i < (int)S.size();i++){
        if (sizeSet[S[i]] > maior) {
            maior = sizeSet[S[i]];
            maiorID = i;
        }
    }
    printf("Set of maximum cardinality is %d\n", maiorID);
    std::vector<int> c1;
    c1.push_back(maiorID);
    std::vector<int> tmpS(S), tmpU(U);
    tmpS.erase(tmpS.begin() + maiorID);
    for (i = 0;i < sizeSet[S[maiorID]];i++){
        tmpU.erase(std::remove(tmpU.begin(), tmpU.end(), sets[S[maiorID]][i]), tmpU.end());
    }
    printf("OLHA O NOVO UNIVERSO: %d\n", c);
    mostraVector(tmpU);
    if (tmpU.size() == 0) {
        if (c < melhor) 
            melhor = c;
    }
    mostraVector(tmpS);
    std::vector<int> result1 = MSC(tmpS, tmpU, c + 1);
    c1.insert(c1.end(), result1.begin(), result1.end());
    std::vector<int> result2 = MSC(tmpS, U, c + 1);
    if ((result1.size() == 1) && (result2.size() == 1) && (result1[0] == -1) && (result2[0] == -1)) {
        std::vector<int> bad;
        bad.push_back(-1);
        return bad;
    }
    return (result1.size() < result2.size()) ? result1 : result2;
}
void createAllSets()
{
    int i;
    for (i = 0;i < n;i++){
        int a;
        printf("(%d,", i);
        sets[i][sizeSet[i]++] = i;
        for (a = 0;a < counter[i];a++){
            printf("%d,", vizinhos[i][a]);
            sets[i][sizeSet[i]++] = vizinhos[i][a];
        }
        printf(")\n");
    }
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
    createAllSets();
    std::vector<int> Universo;
    std::vector<int> S;
    int i;
    for (i = 0;i < n;i++){
        Universo.push_back(i);
        S.push_back(i);
    }
    std::vector<int> res = MSC(Universo, Universo, 0);
    printf("->%d %d\n", res.size(), melhor);
    return 0;
}
