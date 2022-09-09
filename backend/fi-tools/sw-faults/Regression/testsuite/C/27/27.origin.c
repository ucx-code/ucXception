/*
 *Celso Mendes
 *celsom
 *2009109378
 */

#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0
#define GREY -2
#define WHITE -3
#define BLACK -4

int vector[1000][1001];
int n, t, mark;
int dfs[1000], low[1000], color[1000], bridge[1000][1000], p[1000];

void TARJAN(int node){
	int i;
	dfs[node]=++t;
	low[node]=dfs[node];
	color[node]=GREY;

	for(i=1; i<=vector[node][0];i++){

		if(color[vector[node][i]]==WHITE){
			
			p[vector[node][i]]=node;
		
			TARJAN(vector[node][i]);
		
			if(low[node]>low[vector[node][i]])
				low[node]=low[vector[node][i]];
			
			if(low[vector[node][i]] > dfs[node]){
				if(node < vector[node][i]){
					bridge[node][vector[node][i]]=1;
				}else{
					bridge[vector[node][i]][node]=1;
				}
			}
			
		}else if(color[vector[node][i]]==GREY && p[node] != vector[node][i]){
			if(low[node]>dfs[vector[node][i]])
				low[node]=dfs[vector[node][i]];
		}
	}

	color[node]=BLACK;
}

int main(){
	int i, j, x, y, m;
	while(scanf("%d %d", &n, &m)!=EOF){
		for(i=0;i<n;i++){
			for(j=vector[i][0];j>=0;j--)
				vector[i][j]=0;
			color[i]=WHITE;
			p[i]=-1;
			dfs[i]=-1;
		}			
		t=0;
		mark=FALSE;

		for(i=0;i<m;i++){
			scanf("%d %d",&x ,&y);
			vector[x][++(vector[x][0])]=y;
			vector[y][++(vector[y][0])]=x;
		}

		for(i=0; i<n;i++){
			if(color[i]==WHITE)
				TARJAN(i);
		}

		for(i=0; i<n;i++){
			for(j=i+1; j<n; j++){
				if(bridge[i][j]==1){
					printf("%d %d\n", i, j);
					mark=TRUE;
					bridge[i][j]=0;
				}
			}
		}

		if(mark!=TRUE){
			printf("No road\n");
		}
  	}
	return 0;
}

