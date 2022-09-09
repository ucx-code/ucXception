#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int vizinhos[35][34];
int counter[35];
int melhor = 9999;
int estadosP[35][35];
int translator[35][35];
int n;
int estadoCounterT=0;
int maxRecurse = -1;

int countVizinhos(int pos, int n)
{	
	int count = 0;
	int i;
	if (estadosP[estadoCounterT][pos] != 0) return -99999;
	for (i = 0; i < counter[pos]; i++)
	{
		if (estadosP[estadoCounterT][vizinhos[pos][i]] == 0)
			++count;
	}
	if (count != 0)
		return count;
	else return -99999;
}

int compare (const void * a, const void * b)
{
	return countVizinhos(translator[estadoCounterT][*(int*)b],n) -
		countVizinhos(translator[estadoCounterT][*(int*)a],n);
}

int desempate (const void * a, const void * b)
{
	int realA = translator[estadoCounterT+1][*(int*)a];
	int realB = translator[estadoCounterT+1][*(int*)b];
	return realA - realB;
	//return *(int*)a - *(int*)b;
}

int rec(int pos, int n, int count, int estadoCounter, int recurseLevel)
{
	++count;
	if (count > n/2) return -1;
	if (count >= melhor) return -1;

	estadosP[estadoCounter][translator[estadoCounter-1][pos]] = 2;
	int i;
	for (i = 0; i < counter[translator[estadoCounter-1][pos]]; i++)
	{
		//printf("Setting %d to 1\n", vizinhos[translator[estadoCounter-1][pos]][i]);
		estadosP[estadoCounter][vizinhos[translator[estadoCounter-1][pos]][i]] = 1;
	}
	/* Ordena outra vez pelo numero de vizinhos */
	estadoCounterT = estadoCounter; // Set estadoCounter global para o qsort
	qsort(translator[estadoCounter], n, sizeof(int), compare);

	estadoCounterT=estadoCounter;
	int maxi = 1;
	for (i = 1; i < n; i++)
	{
			if (countVizinhos(translator[estadoCounter][0],0) - countVizinhos(translator[estadoCounter][i],0) == 0)
			++maxi;	}

	if (maxi > 1)
	{
		estadoCounterT=estadoCounter-1;
		// Precisamos de desempatar pelo que esta mais a esquerda
		qsort(translator[estadoCounter], maxi, sizeof(int), desempate);	
	}
	bool todosTemAcesso = true;
	// Verifica se nao existem 0's
	for (i = 0; i < n; i++)
		if (estadosP[estadoCounter][i] == 0)
		{
			todosTemAcesso = false;
			break;
		}
	if ((todosTemAcesso) && (count < melhor)) {
		melhor = count; 
		maxRecurse = recurseLevel;
		//printf("Ja ta %d\n", melhor);	
	}
	//bool todosTemAcesso = true;
	for (i = 0; i < maxi; i++)
	{
		//printf("Estado de %d e %d\n",
		if (estadosP[estadoCounter][translator[estadoCounter][i]] == 0)
		{
		//	todosTemAcesso = false;
			memcpy(estadosP[estadoCounter+1], estadosP[estadoCounter], sizeof(int)*35);
			rec(i, n, count, estadoCounter+1, recurseLevel+1);
		}
	}
	
	return 2;
}

int main()
{

	scanf("%d", &n);
	if (n <= 1) {
		printf("%d\n", n);
		return 0;
	}
	int tmpA, tmpB;
	while (scanf("%d %d", &tmpA, &tmpB) != EOF)
	{
		vizinhos[tmpA-1][counter[tmpA-1]] = tmpB-1;
		vizinhos[tmpB-1][counter[tmpB-1]] = tmpA-1;
		counter[tmpA-1]++; counter[tmpB-1]++;
	}

	/* Cria translator */
	int i,a;
	for (a = 0; a < n; a++)
		for (i = 0; i < n; i++)
			translator[a][i] = i;
	/* Ordena translator pelo numero de vizinhos */
	estadoCounterT=0;
	qsort(translator[0], n, sizeof(int), compare);

	/* Ve ate que n vai o numero de valores maximos */
	int maxi=1;	
	for (i = 1; i < n; i++)
		if (countVizinhos(translator[0][i],0) == countVizinhos(translator[0][0],0))
		++maxi;

	for (i = 0; i < maxi; i++)
	{
	//printf("%d\n", translator[0][i]);
		int a;
		for (a = 0; a < 35; a++)
			memset(estadosP[a], 0, sizeof(int)*35);
		rec(i, n, 0, 1, 0);
		
	}
	printf("%d\n", melhor);
	return 0;
}
