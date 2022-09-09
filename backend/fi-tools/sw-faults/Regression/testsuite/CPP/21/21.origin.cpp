#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MEM 1024*1024*5

int vizinhos[35][34];
int counter[35];

unsigned char state[1024*1024*20][35];
unsigned char stateOld[1024*1024*20][35];
int stateCounter = 1;

int n;

int criaEstados(unsigned char * estado)
{
	unsigned char novoEstado[35];
	memcpy(novoEstado, estado, 35*sizeof(char));
	int i;
	for (i = 0; i < n; i++)
	{
		if (estado[i] == 0)
		{
			novoEstado[i] = 2;			
			// Coloca os vizinhos 
			int z;
			for (z=0; z < counter[i]; z++)
				novoEstado[vizinhos[i][z]] = 1;
			memcpy(state[stateCounter++], novoEstado, sizeof(char)*35);	
			memcpy(novoEstado, estado, 35*sizeof(char));
		}
	}
	//state[stateCounter++] = novoEstado;
}

void dfs(int nivel)
{
	// Testa se neste nivel existe uma solucao
	//if (stateCounter == 0) goto nao_Da;	
	int i,a;
	printf("STATECOUNTER %d\n", stateCounter);
	for (i = 0; i < stateCounter; i++)
	{
		bool tudoPreenchido = true;
		for (a = 0; a < n; a++)
		{
			if (state[i][a] == 0)
			{
				tudoPreenchido = false;
				break;
			}
		}
		if (tudoPreenchido == true)
		{
			// Obtemos solucao
			int k;
printf("Estado ");
			for (k = 0; k < n; k++)
			printf("%d", state[i][k]);
			printf("\nCheguei %d\n", nivel);
			return;

		}	
	}

	
	// Cria novos estados
	memcpy(stateOld, state, sizeof(char)*35*MAX_MEM);
	int oldCounter = stateCounter;	
	stateCounter = 0;
	for (i = 0; i < (oldCounter/2)+1; i++)
	{
		criaEstados(stateOld[i]);
	}
	// Aumenta de nivel
	dfs(nivel+1);
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
	int i;
	dfs(0);
	/*for (i = 0; i < n; i++)
	{
		int a;
		for (a = 0; a < 35; a++)
			memset(estadosP[a], 0, sizeof(int)*35);
		rec(i, n, 0, 0);
		
	}*/
	//printf("%d\n", melhor);
	return 0;
}
