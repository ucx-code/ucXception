#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

		//tmpInv[i] = (tmp[i] == '0') ? '1' : '0';
		//tmpInv[i] = '1'-tmp[i]+'0';
		//tmpInv[i] = 97-tmp[i];		
		//tmpInv[i] = tmp[i] ^ 1;
unsigned char matrix[1024*1024];
unsigned char tmp[1024*1024];
unsigned char tmpInv[1024*1024];
		
int quickPow[] = {1,2,4,8,16,32,64,128,256,512,1024};
void rec(int n, int target, int r)
{
	if (n > target) return;
	if (n == 0) 
	{
		matrix[0] = '0';
		rec(n+1, target, r);
	} else 	{
		int halfSquare = quickPow[n-1];
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
	int a;
	// Inverte a primeira metade e copia o resto
	for (i = 0; i <= halfSquare*(halfSquare/2); i++)
		tmpInv[i] =  tmp[i] ^1;
	memcpy(tmpInv+(halfSquare*halfSquare/2)+1, tmpInv, halfSquare*halfSquare/2);

	if (r == 0) {
		for (i = halfSquare/2-1; i <= halfSquare; i++)
			for (a = 0; a <= halfSquare/2; a++)
				tmpInv[i*halfSquare-a] = tmp[i*halfSquare-a]^1;

	} else if (r==1) {
		for (i = 0; i < halfSquare; i++)
			for (a = halfSquare/2; a < halfSquare; a++)
				tmpInv[i*halfSquare+a] =tmp[i*halfSquare+a]^1;
	} else if (r == 2) {
		for (i = 0; i < halfSquare; i++)
			for (a = 0; a < halfSquare; a++)
				tmpInv[i*halfSquare+a] =tmp[i*halfSquare+a]^1;
	} else if (r == 3) {
		for (i = halfSquare/2-1; i <= halfSquare; i++)
			for (a = halfSquare/2; a <= halfSquare; a++)
				tmpInv[i*halfSquare-a] = tmp[i*halfSquare-a]^1;
	}


	for (i = 0; i < halfSquare; i++)
	{
		if (r == 0)
			memcpy(matrix+halfMatrix+((i*2+1)*halfSquare),tmpInv+halfSquare*i , halfSquare);
		else if (r == 1)
			memcpy(matrix+((i*2+1)*halfSquare), tmpInv+halfSquare*i, halfSquare);
		else if (r == 2)
			memcpy(matrix+((i*2)*halfSquare), tmpInv+halfSquare*i, halfSquare);
		else if (r == 3)
			memcpy(matrix+halfMatrix+(i*2*halfSquare), tmpInv+halfSquare*i, halfSquare);	
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
			fwrite(matrix+(i*squared), 1, squared, stdout);
			putchar('\n');
		}
	}
//	fwrite(matrix, 1, 40, stdout);
}

