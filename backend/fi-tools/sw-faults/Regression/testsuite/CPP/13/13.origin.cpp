#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Matrix 2**10
unsigned char matrix[1024*1024];
// Matrix auxiliar 2**9
unsigned char tmp[512*512];
		
// Potencias de 2 pre-calculadas
unsigned int quickPow[] = {1,2,4,8,16,32,64,128,256,512,1024};

// Funcao recursiva
void rec(int n, int target, int r)
{
	// Termina
	if (n > target) return;
	
	// Caso n=0	
	if (n == 0) 
	{
		matrix[0] = '0';
		rec(n+1, target, r);
	} else 	{
	
		int halfSquare = quickPow[n-1];
		int halfSquare2 = halfSquare*halfSquare;
		int halfMatrix = halfSquare2*2;
		register int i;
		
		// Copia a matrix do n anterior para tmp.
		memcpy(tmp, matrix, halfSquare2);

		
		// Linha a linha. Preenche a primeira metade com os valores da matrix do n anterior
		for (i = 0; i < halfSquare; i++)
		{
               
			memcpy(matrix+((i*2)*halfSquare), tmp+halfSquare*i, halfSquare);	
			memcpy(matrix+((i*2+1)*halfSquare), tmp+halfSquare*i, halfSquare);
		}

		// Copia a metade de cima para baixo
		memcpy(matrix+halfMatrix, matrix, halfMatrix);

		// Inverte a matrix do n anterior
		// ord('1') = 49; ord('0') = 48
		// bin(49) = 110001; bin(48) = 110000;
		// '1' ^ 1 = '0'; '0' ^ 1 = '1'
		for (i = 0; i < halfSquare2; i+=4)
		{
			//tmpInv[i] = (tmp[i] == '0') ? '1' : '0';
			//tmpInv[i] = '1'-tmp[i]+'0';
			//tmpInv[i] = 97-tmp[i];
			register unsigned int *pont = (unsigned int *)(tmp+i);
			*pont = *pont ^ 0x01010101;
			
		}

		// Itera linha a linha pela metade de baixo. E consoante o valor de r preenche com a matrix invertida
		for (i = 0; i < halfSquare; i++)
		{
			switch (r)  {
				case 0:	memcpy(matrix+halfMatrix+((i*2+1)*halfSquare),    tmp+halfSquare*i, halfSquare); 	break;
				case 1:	memcpy(matrix+((i*2+1)*halfSquare),               tmp+halfSquare*i, halfSquare);	break;
				case 2:	memcpy(matrix+((i*2)*halfSquare),                 tmp+halfSquare*i, halfSquare);	break;
				case 3:	memcpy(matrix+halfMatrix+(i*2*halfSquare),        tmp+halfSquare*i, halfSquare);	break;
			}
		}

		// Chama o proximo n
		rec(n+1, target, r);
	}
}

int main()
{
	unsigned int n, r;
	register unsigned int i, squared;
		
	while (scanf("%d %d", &n, &r) != EOF)
	{
		squared = quickPow[n];

		rec(0,n,r%4);

		// Linha a linha escreve o resultado
		for (i = 0; i < squared; i++)
		{
			fwrite(matrix+(squared*i), 1, squared, stdout);
			putchar('\n');		
		}
	}
}


