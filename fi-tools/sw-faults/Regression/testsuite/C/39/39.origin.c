/*
 *Celso Mendes
 *celsom
 *2009109378
 */

#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0

int vector[501][501];
int color[501];
int no,n;

/*Algoritmo Graph Coloring Problem*/
int GCP(int v){
	int i, j, pos;
	if(v == n+1)
		return TRUE;
	

	for(i=1; i<=2; i++){
		pos =TRUE;

		/*Precorre todos os vertices adjacentes*/
		for(j=1; j<=vector[v][0]; j++){
		
			if(color[vector[v][j]]==i){
				/*Torna-se impossivel usar esta cor*/
				pos = FALSE;
				break;
			}
		}

		if(pos==TRUE){
			color[v]=i;
			if(GCP(v+1)==TRUE)
				return TRUE;
			color[v]=0;
		}
	}

	return FALSE;	
}


int main(){
	int i, j, x, y, m;
	while(scanf("%d %d", &n, &m)!=EOF){
		for(i=1;i<=n;i++){
			color[i]=0;
			for(j=vector[i][0];j>=0;j--)
				vector[i][j]=0;
		}

		/*
		*Lista de adjacência
		*Posicção zero da tabela
		*/
		for(i=0;i<m;i++){
			scanf("%d %d",&x ,&y);
			vector[x][(++vector[x][0])]=y;
			vector[y][(++vector[y][0])]=x;
		}

		if(GCP(1)==TRUE)
			printf("NOT SURE\n");
		else
			printf("NO\n");
  		
  	}
	return 0;
}

