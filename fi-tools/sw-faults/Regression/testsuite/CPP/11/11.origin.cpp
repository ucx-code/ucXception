/**
Versao dos 26ms
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

unsigned char matrix[1024*1024];
unsigned char tmp[512*512];
		
int quickPow[] = {1,2,4,8,16,32,64,128,256,512,1024};
void rec(int n, int target, int r)
{
	if (n > target) return;
	if (n == 0) 
	{
		matrix[0] = '0';
		rec(n+1, target, r);
	} else 	{
		int halfSquare = quickPow[n-1]; // OPTIMIZE
		int halfSquare2 = halfSquare*halfSquare;
		int halfMatrix = halfSquare2*2;
		int i;
		
		memcpy(tmp, matrix, halfSquare2);

		
	
	for (i = 0; i < halfSquare; i++)
	{
		memcpy(matrix+((i*2)*halfSquare), tmp+halfSquare*i, halfSquare);	
		memcpy(matrix+((i*2+1)*halfSquare), tmp+halfSquare*i, halfSquare);
	}
	memcpy(matrix+halfMatrix, matrix, halfMatrix);

	for (i = 0; i < halfSquare2; i+=8)
	{
		//tmpInv[i] = (tmp[i] == '0') ? '1' : '0';
		//tmpInv[i] = '1'-tmp[i]+'0';
		//tmpInv[i] = 97-tmp[i];
		unsigned long *lel = (unsigned long *)(tmp+i);
		*lel = *lel ^ 0x0101010101010101;
		//tmpInv[i] = tmp[i] ^ 1;		
	}

	for (i = 0; i < halfSquare; i++)
	{
		if (r == 0)
			memcpy(matrix+halfMatrix+((i*2+1)*halfSquare),tmp+halfSquare*i , halfSquare);
		else if (r == 1)
			memcpy(matrix+((i*2+1)*halfSquare), tmp+halfSquare*i, halfSquare);
		else if (r == 2)
			memcpy(matrix+((i*2)*halfSquare), tmp+halfSquare*i, halfSquare);
		else if (r == 3)
			memcpy(matrix+halfMatrix+(i*2*halfSquare), tmp+halfSquare*i, halfSquare);	
	}

		
		
		rec(n+1, target, r);
	}
}

int main()
{
	int n, r;
	int i, squared;
		
	while (scanf("%d %d", &n, &r) != EOF)
	{
		squared = quickPow[n];
		//halfSquare = quickPow[n-1];
		rec(0,n,r%4);

		for (i = 0; i < squared; i++)
		{
			//char tmp[1026*1024];
			//strncpy(tmp, matrix+(int)(i*pow(2,n)), pow(2,n));
			//memcpy(tmp, matrix+(i*squared), squared);
			//tmp[squared] = '\0';			
			//matrix[i*squared+1] = '\n';
				//printf("%s\n", tmp);
			//fputs(tmp, stdout);
			fwrite(matrix+(squared*i), 1, squared, stdout);
			putchar('\n');		
		}
	}
//	fwrite(matrix, 1, 40, stdout);
}


