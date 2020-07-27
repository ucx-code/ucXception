/**
 * Celso Mendes
 * 2009109378
 **/

#include <stdio.h>
#include <stdlib.h>

int pontos[999][2];
int n;

int determinante(int p1, int p2, int p3){
	int det = (pontos[p1][0]*pontos[p2][1]+pontos[p2][0]*pontos[p3][1]+pontos[p3][0]*pontos[p1][1])-
	(pontos[p1][1]*pontos[p2][0]+pontos[p2][1]*pontos[p3][0]+pontos[p3][1]*pontos[p1][0]);
	return det;
}

float sign(int p1, int p2, int p3)
{
  return (pontos[p1][0] - pontos[p3][0]) * (pontos[p2][1] - pontos[p3][1]) - (pontos[p2][0] - pontos[p3][0]) * (pontos[p1][1] - pontos[p3][1]);
}

int pontoInt(int pt, int v1, int v2, int v3)
{
	int b1=0, b2=0, b3=0;
  	
  	if(sign(pt, v1, v2) < 0)
  		b1=1;
  	if(sign(pt, v2, v3) < 0)
  		b2=1;
  	if(sign(pt, v3, v1) < 0)
  		b3=1;

  	if(b1==b2 && b2==b3)
  		return 1;

  	return 0;
}

int checkIn(int p1, int p2, int p3){
	int i;

	for(i=0; i<n; i++){
		if(i!=p1 && i!=p2 && i!=p3){
			if(pontoInt(i, p1, p2, p3)==1){
				return 0;
			}
		}
	}
	return 1;
}

int main(){
	int i, count=0;
	
	scanf("%d", &n);

	for (i=0;i<n; i++){
		scanf("%d %d", &pontos[i][0], &pontos[i][1]);
	}

	for (i=0;i<n; i++){
		if(determinante(i%n,(i+1)%n,(i+2)%n)<0){
			if(checkIn(i%n,(i+1)%n,(i+2)%n)==1){
				count++;
			}
		}	
	}

	printf("%d\n", count);

	return 0;
}
