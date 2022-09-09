#include <stdio.h>

char map[100][101];
short path[10000][2];
short nlines;
short ncols;

/*Linha,coluna a procurar; caracter esperado; numero de posicoes por onde passa a possibilidade de caminho;
 *x,y ultima posicao por onde passou */
short checkPath(short line, short col, char car, short count, short x, short y){

	/*Esta funcao retorna 0 se nao existir caminho, e retorna 1 se existir um caminho possivel ate ao fim do mapa*/
	/*Verifica se a posicao encontra-se dentro dos limites*/
	if(col>=0 && col<ncols && line>=0 && line<nlines){
		
		/*Verifica se o char encontrado é = ao esperado*/
		if(map[line][col]==car){
			
			/*Verifica se chegamos ao fim mapa*/
			if(line==(nlines-1)){
				
				/*grava a posicao de passagem no vector*/
				path[count][0]=line+1;
				path[count][1]=col+1;
				return 1;
			}
			/*verifica cada posicao conforme a barra*/
			else if(map[line][col]=='\\'){
				
				if(checkPath(line+1, col, '/', count+1, line, col)==1){
					path[count][0]=line+1;
					path[count][1]=col+1;
					return 1;
				}
				if(checkPath(line+1, col+1, '\\', count+1, line, col)==1){
					path[count][0]=line+1;
					path[count][1]=col+1;
					return 1;
				}
				if(checkPath(line+1, col+1, '|', count+1, line, col)==1){
					path[count][0]=line+1;
					path[count][1]=col+1;
					return 1;
				}
				/*Verifica se a proxima posicao e uma posicao repetida*/
				if((line != x) || (col+1 != y))
					if(checkPath(line, col+1, '/', count+1, line, col)==1){
						path[count][0]=line+1;
						path[count][1]=col+1;
						
						return 1;
					}
				if((line != x) || (col-1 != y)){
					if(checkPath(line, col-1, '/', count+1, line, col)==1){
						path[count][0]=line+1;
						path[count][1]=col+1;
						return 1;
					}
				}
			}
			else if(map[line][col]=='/'){
				
				if(checkPath(line+1, col-1, '|', count+1, line, col)==1){
					path[count][0]=line+1;
					path[count][1]=col+1;
					return 1;
				}
				if(checkPath(line+1, col-1, '/', count+1, line, col)==1){
					path[count][0]=line+1;
					path[count][1]=col+1;
					return 1;
				}
				if(checkPath(line+1, col, '\\', count+1, line, col)==1){
					path[count][0]=line+1;
					path[count][1]=col+1;
					return 1;
				}
				if ((line != x) || (col+1!=y))
					if(checkPath(line, col+1, '\\', count+1, line, col)==1){
						path[count][0]=line+1;
						path[count][1]=col+1;
						return 1;
					}
				if((line != x) || (col-1 != y))
					if(checkPath(line, col-1, '\\', count+1, line, col)==1){
						path[count][0]=line+1;
						path[count][1]=col+1;
						return 1;
					}
			}
			else if(map[line][col]=='|'){
				if(checkPath(line+1, col-1, '/', count+1, line, col)==1){
					path[count][0]=line+1;
					path[count][1]=col+1;
					return 1;
				}
				if(checkPath(line+1, col, '|', count+1, line, col)==1){
					path[count][0]=line+1;
					path[count][1]=col+1;
					return 1;
				}
				if(checkPath(line+1, col+1, '\\', count+1, line, col)==1){
					path[count][0]=line+1;
					path[count][1]=col+1;
					return 1;
				}
			}
			/*Caso o caminho nao tenha saida elimina a posicao*/
			map[line][col]='#';
		}
	}
	return 0;
}	

int main(){
	short line=0, col=0, count=0, aux, i;
	/*Le o numero de linhas e colunas do mapa*/
	while(scanf("%hd %hd", &nlines, &ncols)!=EOF){
		
		/*Le o mapa*/
		for(i=0; i<nlines; i++){
			scanf("%s", map[i]);
		}
		
		/*Verifica o caminho a partir das diferentes possibilidades da primeira linha */
		for(col=0; col<ncols;col++){
			if(map[0][col]=='\\'){
				if(checkPath(line+1, col, '/', count+1, line, col)==1){
					aux=1;
				}else if(checkPath(line+1, col+1, '\\', count+1, line, col)==1){
					aux=1;
				}else if(checkPath(line+1, col+1, '|', count+1, line, col)==1){
					aux=1;
				}
			}else if(map[0][col]=='/'){
				if(checkPath(line+1, col, '\\', count+1, line, col)==1){
					aux=1;
				}else if(checkPath(line+1, col-1, '|', count+1, line, col)==1){
					aux=1;
				}else if(checkPath(line+1, col-1, '/', count+1, line, col)==1){
					aux=1;
				}
			}else if(map[0][col]=='|'){
				if(checkPath(line+1, col-1, '/', count+1, line, col)==1){
					aux=1;
				}else if(checkPath(line+1, col, '|', count+1, line, col)==1){
					aux=1;
				}else if(checkPath(line+1, col+1, '\\', count+1, line, col)==1){
					aux=1;
				}
			}
			
			/*Guarda a posicao inicial do caminho*/
			if(aux==1){
				path[0][0]=1;
				path[0][1]=col+1;
				break;
			}
		}
		
		/*Imprime o resultado da procura pelo mapa*/
		if(aux==0){
			printf("No Path!\n");
		}else{
			i=1;
			printf("(%hd,%hd)",path[0][0], path[0][1]);
			while(path[i][0]!=0){
				printf(",(%hd,%hd)",path[i][0], path[i][1]);
				path[i][0]=0;
				path[i][1]=0;
				i++;
			}
			printf("\n");
			aux=0;	
		}	
	}
	return 0;
}
