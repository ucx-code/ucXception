#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 101*100 = ~10Kb
#define BUFFER_SIZE 10120

// Buffer que armazena a matrix lida do stdin. Inclui \n no final de cada linha.
// O leftmost bit e utilizado como indicador se a celula ja foi visitada
unsigned char buffer[BUFFER_SIZE];

// Retorna a posicao no eixo dos x 
inline int getX(int pos, int width)
{
	return pos%(width+1)+1;
}

// Retorna a posicao no eixo dos y
inline int getY(int pos, int width)
{
	return pos/(width+1)+1;
}

// Indica se e necessario concatenar uma , no final do printf
int recurseSize = 0;

// Funcao recursiva que executa DFS no "grafo"
// Começa do fim para o inicio de forma a evitar o uso de uma estrutura para armazenar o caminho
// Aproveita o leftmost bit ser alterado aquando de uma visita para executar essa verificacao
// ao mesmo tempo que verifica o valor das celulas adjacentes
int recurse(int pos, int max, int width)
{
	// Verifica se estamos na ultima linha
	if (pos <= width)
	{
		printf("(%d,%d)",  getY(pos, width), getX(pos, width));
		return 1;
	}

	++recurseSize;	

	// Marca esta celula como visitada
	buffer[pos] |= 0x80;

	// A posicao actual necessita de binary-or 0x80 por causa da instrucao anterior
	// Os seguintes if's nas celulas vizinhas verificam o caracter que la se encontra (/,|,\)
	// e se ja foi visitado ao mesmo tempo. ( ('|' | 0x80) != '|')
	/* |  | |
	   | /   \  */
	if (buffer[pos] == ('|' | 0x80))
	{
		if (buffer[pos-width] == '/')
		{
			if (recurse(pos-width, max, width) == 1) 
			{
				if (recurseSize != 0) printf(",");
				printf("(%d,%d)",  getY(pos, width), getX(pos, width));				
				return 1;
			}

		}
		if (buffer[pos-width-1] == '|')
		{
			if (recurse(pos-width-1, max, width) == 1) 
			{
				if (recurseSize != 0) printf(",");
				printf("(%d,%d)",  getY(pos, width), getX(pos, width));	
				return 1;
			}
		}
		if (buffer[pos-width-2] == '\\')
		{
			if (recurse(pos-width-2, max, width) == 1) 
			{
				if (recurseSize != 0) printf(",");
				printf("(%d,%d)",  getY(pos, width), getX(pos, width));	
				return 1;
			}
		}
	}
	/* \  \  \  
            |  \ / */
	if ((buffer[pos] == ('\\' | 0x80))) {
		if ((buffer[pos-width-2] == '|') || (buffer[pos-width-2] == '\\'))
		{
			if (recurse(pos-width-2, max, width) == 1) 
			{
				if (recurseSize != 0) printf(",");	
				printf("(%d,%d)",  getY(pos, width), getX(pos, width));

				return 1;
			}
		}
		if (buffer[pos-width-1] == '/')
		{
			if (recurse(pos-width-1, max, width) == 1) 
			{
				if (recurseSize != 0) printf(",");
				printf("(%d,%d)",  getY(pos, width), getX(pos, width));
	
				return 1;
			}
		}
		/* \/\  */
		if ((buffer[pos-1] == '/') && (buffer[pos-2] == '\\')) {
			if (recurse(pos-2, max, width) == 1) 
			{
				if (recurseSize != 0) printf(",");	
				printf("(%d,%d),(%d,%d)",  getY(pos-1, width), getX(pos-1, width),
				                 	   getY(pos, width),   getX(pos, width) );

				return 1;
			}
		}
		
	}
	
	/* /  /  / 
	  |  /   \ */
	if (buffer[pos] == ('/' | 0x80)) {
		if ((buffer[pos-width] == '|') || (buffer[pos-width] == '/'))
		{
			if (recurse(pos-width, max, width) == 1) 
			{
				if (recurseSize != 0) printf(",");
				printf("(%d,%d)",  getY(pos, width), getX(pos, width));
	
				return 1;
			}
		}
		if (buffer[pos-width-1] == '\\')
		{
			if (recurse(pos-width-1, max, width) == 1) {
			
				if (recurseSize != 0) printf(",");
				printf("(%d,%d)",  getY(pos, width), getX(pos, width));
	
				return 1;
			}
		}
		/* \/\  */
		if ((buffer[pos+1] == '\\') && (buffer[pos+2] == '/')) {
			if (recurse(pos+2, max, width) == 1) 
			{
				if (recurseSize != 0) printf(",");
				printf("(%d,%d),(%d,%d)",  getY(pos+1, width), getX(pos+1, width),
				                 	   getY(pos, width),   getX(pos, width) );
	
				return 1;
			}
		}
		
	}

	// Dead-End
	return -1;
}

int main()
{
	int i, width, height, matrixSize;
	while (true)
	{	
		if (scanf("%d %d\n", &height, &width) != 2) return 1;
		
		matrixSize = (width+1)*height;		

		// Le a matrix de uma so vez
		fread(buffer, 1, matrixSize, stdin); 		

		// Procura saidas na ultima linha
		for (i = matrixSize-width-1; i < matrixSize; i++)
		{
		
			if ((buffer[i] == '|') || (buffer[i]== '\\') || (buffer[i]== '/')) 
			{
				// Faz uma procura a comecar da saida
				recurseSize = 0;
				if (recurse(i, matrixSize, width) == 1)
					goto terminei_matrix_sucesso;			
			}	
		}
		
		// Nao encontrou
		printf("No Path!");
		
		terminei_matrix_sucesso:
		printf("\n");
	} 

	return 0;
}

