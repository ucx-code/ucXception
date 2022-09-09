#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int vizinhos[35][34];
int counter[35];
int melhor = 9999;
int estadosP[35];

int taTudo(int n)
{
	int i;
	for (i = 0; i < n; i++)
		if (estadosP[i] == 0) return 0;
	return 1;
}

int rec(int pos, int n, int count)
{
	++count;
	if (count > (n/3)+1) return -1;
	if (count >= melhor) return -1;
	
	estadosP[pos] = 2;
	int i;
	for (i = 0; i < counter[pos]; i++)
	{
		estadosP[vizinhos[pos][i]] = 1;
	}
if (taTudo(n) == 1) melhor = count;
	for (i = pos+1; i < n; i++)
	{
		if (estadosP[i] == 0)
		{
			rec(i, n, count);
		}
	}

	for (i = 0; i < counter[pos]; i++)
	{
		estadosP[vizinhos[pos][i]] = 0;
	}

	return 1;
}

int main()
{

	int n;
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
	int i;
	for (i = 0; i < n; i++)
	{
		rec(i, n, 0);
		
	}
	printf("%d\n", melhor);
	return 0;
}
