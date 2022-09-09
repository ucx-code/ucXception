#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char matrix[1024*1024];
unsigned char tmpInv[1024*1024];
unsigned char tmp[512*512];

const unsigned int quickPow[] = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65535,131072,262144};
void rec(unsigned int n, unsigned int target, unsigned int r)
{
	if (n > target) return;
	if (n == 0) 
	{
		matrix[0] = '0';
		rec(n+1, target, r);
	} else 	{
		unsigned int halfSquare = quickPow[n-1];
		unsigned int halfSquare2 = quickPow[(n-1)*2];//halfSquare*halfSquare;
 		unsigned int halfMatrix = halfSquare*halfSquare*2;
		unsigned int i;
		
		memcpy(tmp, matrix, halfSquare*halfSquare);

		unsigned long *lel, *lal;
	for (i = 0; i < halfSquare2; i+=4)
	{
		//tmpInv[i] = (tmp[i] == '0') ? '1' : '0';
		//tmpInv[i] = '1'-tmp[i]+'0';
		//tmpInv[i] = 97-tmp[i];
		lel = (unsigned long *)(tmp+i);
		lal = (unsigned long *)(tmpInv + i);
		*lal = *lel ^ 0x01010101;
		
	}

		for (i = 0; i < halfSquare; i++)
		{
		if (r == 2)
			memcpy(matrix+((i*2)*halfSquare), tmpInv+halfSquare*i, halfSquare); else
memcpy(matrix+((i*2)*halfSquare), tmp+halfSquare*i, halfSquare);	
		if (r == 1)
			memcpy(matrix+((i*2+1)*halfSquare), tmpInv+halfSquare*i, halfSquare);
		else memcpy(matrix+((i*2+1)*halfSquare), tmp+halfSquare*i, halfSquare);
		}
		for (i = 0; i < halfSquare; i++)
		{
		if (r == 3)
			memcpy(matrix+halfMatrix+(i*2*halfSquare), tmpInv+halfSquare*i, halfSquare);
		else
			memcpy(matrix+halfMatrix+(i*2*halfSquare), tmp+halfSquare*i, halfSquare);
		if (r == 0)
		{
		memcpy(matrix+halfMatrix+((i*2+1)*halfSquare),tmpInv+halfSquare*i , halfSquare);
		} else {
		memcpy(matrix+halfMatrix+((i*2+1)*halfSquare),tmp+halfSquare*i , halfSquare);
		}
		}


		
		
		rec(n+1, target, r);
	}
}

int main()
{
	unsigned int n, r;
	unsigned int i;
		
	while (scanf("%d %d", &n, &r) != EOF)
	{
		unsigned int squared = quickPow[n];
		rec(0,n,r%4);

		for (i = 0; i < squared; i++)
		{
			fwrite(matrix+(squared*i), 1,squared,stdout);			
			putchar('\n');	
		}
	}
//	fwrite(matrix, 1, 40, stdout);
}

