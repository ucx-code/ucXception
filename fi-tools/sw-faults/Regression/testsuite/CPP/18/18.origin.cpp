/**
Usa getchar
Remove o converter 
Juntar o hitCount no leftmost bit do buffer
Reordered ifs
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stack>

// 101*100+20 = 10120 ~10Kb
#define BUFFER_SIZE 10120

char buffer[BUFFER_SIZE];

int getX(int pos, int width)
{
	return pos%(width+1)+1;
}

int getY(int pos, int width)
{
	return pos/(width+1)+1;
}

std::stack<int> answer;
int recurse(int pos, int max, int width)
{
	// Verify if we have been here already
	if ((buffer[pos] & 0x80 ) == 0x80) return -1;
	buffer[pos] |= 0x80;
	//if (hitCount[pos] == iteracao) return -1;
	//hitCount[pos] = iteracao;

	// Verify if we are on the last line
	if ((pos >= max - width -1) && (pos < max)) 
	{
		answer.push(pos);
		return 1;
	}
	int bufferPos = (buffer[pos]& 0x7F);
	int bufferPosWidth =(buffer[pos+width]& 0x7F);
	int bufferPosWidth1 = (buffer[pos+width+1]& 0x7F);
	int bufferPosWidth2 = (buffer[pos+width+2]& 0x7F);
	int bufferPos1 = (buffer[pos+1]& 0x7F);
	int bufferPos2 = (buffer[pos+2]& 0x7F);
	int bufferPosN1 = (buffer[pos-1]& 0x7F);
	int bufferPosN2 = (buffer[pos-2]& 0x7F);	
	// No need to verify if leftmost/rightmost because of /n padding

	/* |  | |
	   | /   \  */

	if (bufferPos == '|')
	{
		if (bufferPosWidth == '/')
		{
			if (recurse(pos+width, max, width) == 1) 
			{
				answer.push(pos);
				return 1;
			}
		}
		if (bufferPosWidth1 == '|')
		{
			if (recurse(pos+width+1, max, width) == 1) 
			{
				answer.push(pos);
				return 1;
			}
		}
		if (bufferPosWidth2 == '\\')
		{
			if (recurse(pos+width+2, max, width) == 1) 
			{
				answer.push(pos);
				return 1;
			}
		}
	}
	/* \  \  \  
            |  \ / */
	if ((bufferPos == '\\')) {
		if ((bufferPosWidth2 == '|') || (bufferPosWidth2 == '\\'))
		{
			if (recurse(pos+width+2, max, width) == 1) 
			{
				answer.push(pos);
				return 1;
			}
		}
		if (bufferPosWidth1 == '/')
		{
			if (recurse(pos+width+1, max, width) == 1) 
			{
				answer.push(pos);
				return 1;
			}
		}
		/* \/\  */
		if ((bufferPos1 == '/') && (bufferPos2 == '\\')) {
			if (recurse(pos+2, max, width) == 1) 
			{
				answer.push(pos+1);
				answer.push(pos);
				return 1;
			}
		}
		
	}
	
	/* /  /  / 
	  |  /   \ */
	if (bufferPos == '/') {
		if ((bufferPosWidth == '|') || (bufferPosWidth == '/'))
		{
			if (recurse(pos+width, max, width) == 1) 
			{
				answer.push(pos);
				return 1;
			}
		}
		if (bufferPosWidth1 == '\\')
		{
			if (recurse(pos+width+1, max, width) == 1) 
			{
				answer.push(pos);
				return 1;
			}
		}
		/* \/\  */
		if ((bufferPosN1 == '\\') && (bufferPosN2 == '/')) {
			if (recurse(pos-2, max, width) == 1) 
			{
				answer.push(pos-1);
				answer.push(pos);
				return 1;
			}
		}
		
	}

	// Dead-End
	return -1;
}

int main()
{
	int i, width, height;
	while (true)
	{	
		if (scanf("%d %d\n", &height, &width) != 2) return 0;
		
		//fflush(stdin);
		int matrixSize = (width+1)*height;		
		for (i = 0; i < matrixSize; i++)
		{	buffer[i] = getchar();
			//scanf("%c", &buffer[i]);		
		}

		//Search for entrances on the first line
		bool encontrou = false;
		for (i = 0 ; i < width; i++)
		{
		
			if (((buffer[i]& 0x7F) == '|') || ((buffer[i]& 0x7F)== '\\') || ((buffer[i]& 0x7F) == '/')) 
			{
	
				int result = recurse(i, matrixSize, width);
				if (result == 1)
				{
					int tamanhoCaminho = answer.size();
					int l;
					for (l = 0; l < tamanhoCaminho; l++) {
						if (l != 0) printf(",");
						int posTmp = answer.top();
						printf("(%d,%d)",  getY(posTmp, width), getX(posTmp, width)); 
						//printf("%d\n", posTmp);
		answer.pop();}
					printf("\n");
					
					encontrou = true;
					break;
				}
			
			
			}	
			if (encontrou) break;
			
		}
		if (!encontrou) printf("No Path!\n");
	} 

	return 0;
}

