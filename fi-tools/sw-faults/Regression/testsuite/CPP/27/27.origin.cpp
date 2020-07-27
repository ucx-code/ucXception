#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int vizinhos[36][36];
int counter[36];

int visit[36];
int best=999999;

	int n;
int saoVizinhos(int i, int j)
{
	int a;

	for (a = 0; a < counter[i]; a++)
		if (vizinhos[i][a] == j)
			return 1;
	return 0;
}
int chegaPaTodos()
{
	int i,a;
	int marca[36];
	memset(marca, 0, sizeof(int)*36);
	//int count = 0;	
	for (i = 1; i <= n; i++)
	{
		if (visit[i] == 1)
		{//	++count;
			marca[i] = 1;		
			for (a = 0; a < counter[i]; a++)
				{// ++count;
				 marca[vizinhos[i][a]] = 1; }
		}
	}
	for (i = 1; i <= n; i++)
		if (marca[i] == 0) return 0;
	//printf("C %d\n", count);
	//if (count >= n) return 1;	
	return 1;

}
void mis(int v, int size)
{
	//if (size > best)
	//	best = size;
	int k = chegaPaTodos();	
	if ((k == 1) && (size < best)) {
		best = size; }
	int i, j;
	for (i = v+1; i <= n; i++)
	{
		bool feasible = true;
		for (j = 1; j <= v; j++)
		{
			if ((visit[j] == 1) && (saoVizinhos(i,j)==1))
			{
				feasible = false;
				break; 
			}
		}
		if (feasible == true)
		{
			visit[i] = 1;
			mis(i, size+1);
			visit[i] = 0;
		}
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
	while (scanf("%d %d", &tmpA, &tmpB) != EOF)
	{
		vizinhos[tmpA][counter[tmpA]] = tmpB;
		vizinhos[tmpB][counter[tmpB]] = tmpA;
		counter[tmpA]++; counter[tmpB]++;
	}
	mis(0,0);
	printf("%d\n", best);
	return 0;
}
