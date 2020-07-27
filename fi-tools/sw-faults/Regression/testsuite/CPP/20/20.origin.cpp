#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int vizinhos[35][35];
int counter[35];
int visit[36];
int best=999999;

int n;

int verifica()
{
	//int count = 0;
	int i,a;
	int tmp[36];

	// Copiamos o array que ja temos para o tmp
	memcpy(tmp, visit, sizeof(int) * 36);
	
	for (i = 0; i < n ; i++)
	{
		// Se ja todos os nodes estao abrangidos
		//if (count == n)  return 0; 

		// Se temos uma printer entao vamos ver os nossos vizinhos
		if (visit[i] == 1)
		{
			//++count;
			
			// Vai ver as ligacoes
			for (a = 0; a < counter[i]; a++)
				//if (tmp[vizinhos[i][a]] == 0)
				//{
					tmp[vizinhos[i][a]] = 1;
					//++count;
				//}
	
		}
	}

	// Se ja todos os nodes estao abrangidos
	//if (count == n) return 0;
	for (i = n-1; i  >= 0; i--) if (tmp[i] == 0) return -1;
	return 0;
}

void rec(int begin, int len)
{
	// Nao vamos melhorar
	if (len >= best)
		return;

	// Verifica se todos os computadores ja estao abrangidos
	if (verifica() == 0)
	{
		if (len < best)
			best = len;
		return; // Nao vamos melhorar adicionando mais 
	}
	int i;

	for (i = begin+1; i < n; i++)
	{
			visit[i] = 1;
			rec(i, len+1);
			visit[i] = 0;	
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
		vizinhos[tmpA-1][counter[tmpA-1]++] = tmpB-1;
		vizinhos[tmpB-1][counter[tmpB-1]++] = tmpA-1;
	}
	
	rec(-1, 0);
	printf("%d\n", best);
	return 0;
}
