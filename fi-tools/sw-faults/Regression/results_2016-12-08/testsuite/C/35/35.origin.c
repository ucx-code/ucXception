#include <stdio.h>

/*Joao Pedro - 2009112620 - joaoap*/
			
int v[301][30001];

int main(){
	int c;		/*numero de disciplinas por curso*/
	int n, m;	/*n - numero de departamentos, m - nr ECTS maximos*/
	int i, j;
	int custo, valor, dept;
	
	scanf("%d %d",&n,&m);
	
	for(dept=1;dept<=n;dept++)
	{
		scanf("%d",&c);
		for (i = 0; i < c; i++)
		{
			scanf("%d %d",&custo,&valor);

			for (j=1;j<=m;j++)
			{
					if(i==0)
					{

						if(custo>j)
							v[dept][j]=v[dept-1][j];
						else{
							if(v[dept-1][j]>valor+v[dept-1][j-custo])
								v[dept][j] = v[dept-1][j];
							else
								v[dept][j] = valor+v[dept-1][j-custo];
						}
					}
					else
					{
						if(v[dept][j]<valor+v[dept-1][j-custo] && custo<j)
							v[dept][j] = valor+v[dept-1][j-custo];
					}
			}
		}
	}

	printf("%d\n", v[n][m]);
	
	return 0;
}