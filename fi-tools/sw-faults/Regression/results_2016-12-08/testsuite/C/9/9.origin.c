/**
  * Celso Mendes 2009109378
  * Saul Santos 2009118728
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int checkNumber(int x);
void printTable(int **table, int lines, int columns);
int **copyTable(int **table, int lines, int columns);

int **orderByLines(int **table, int lines, int columns);
int rearranjaLines(int vect[], int li, int ls);
void QsortLines(int v[], int li, int ls);

void QsortCol(int v[], int li, int ls);
int rearranjaCol(int vect[], int li, int ls);
int **orderByColumns(int **table, int lines, int columns);


int main(int argc, char *argv[]){
    int lines, columns;
    int i, j;
    int **table, **tableLines, **tableColumns;

    scanf("%d %d", &lines, &columns);
    table = (int **) malloc(lines*sizeof(int*));

    for(i=0; i<lines; i++){
        table[i]=(int *)malloc(columns*sizeof(int));
        for(j=0;j<columns; j++){
            scanf("%d", &table[i][j]);
            if(checkNumber(table[i][j]))
                j--;
        }
    }

    tableColumns = orderByColumns(table, lines, columns);
    tableLines = orderByLines(table, lines, columns);

    /*Imprimir tabelas*/
    printTable(table, lines, columns);
	printf("\n");
    printTable(tableLines, lines, columns);
	printf("\n");
    printTable(tableColumns, lines, columns);

    for(i=0; i<lines; i++){
        free(table[i]);
        free(tableLines[i]);
        free(tableColumns[i]);
    }

    free(table);
    free(tableLines);
    free(tableColumns);

    return 0;
}

int checkNumber(int x){
    /*Verifica se um numero e valido*/
    int z=1;
    if (x>=10 && x<=99){
        z=0;
    }
    return z;
}

void printTable(int **table, int lines, int columns){
    int i, j;
    /*Imprime a tabela*/
    for(i=0; i<lines; i++){
        for(j=0;j<columns; j++){
            if(j!=(columns-1))
                printf("%d-", table[i][j]);
            else
                printf("%d", table[i][j]);
        }
        putchar('\n');
    }
}

/*Ordenar por linhas*/
int **orderByLines(int **table, int lines, int columns){
    int **tableLines = copyTable(table, lines, columns);
    int i;
    for (i=0; i<lines; i++){
        QsortLines(tableLines[i], 0, columns-1);
    }
    return tableLines;
}

/*QuickSort --- Linhas*/
void QsortLines(int v[], int li, int ls){
	int j;
	if (li < ls){
		j = rearranjaLines(v,li, ls);
		QsortLines(v, li, j-1);
		QsortLines(v, j+1, ls);
	}
}

int rearranjaLines(int vect[], int li, int ls){
	int a, down, up, temp;
	a=vect[li];
	down=li;
	up=ls;
	while(down<up){
		while(vect[down]<=a && down<ls)
			down++;
		while(vect[up]>a)
			up--;
		if(down < up){
			temp=vect[down];
			vect[down]=vect[up];
			vect[up]=temp;
		}
	}
	vect[li]=vect[up];
	vect[up]=a;
	return up;
}

/*Ordenar por colunas*/
int **orderByColumns(int **table, int lines, int columns){
    int **tableCol = copyTable(table, lines, columns);
    int *vect;
    int i, j;
    
    vect = (int *)malloc(lines*sizeof(int));
    for (i=0; i<columns; i++){
        for(j=0; j<lines; j++){
			vect[j]=tableCol[j][i];
		}
        QsortCol(vect, 0, lines-1);
        for(j=0; j<lines; j++){
			tableCol[j][i]=vect[j];
		}
    }
    free(vect);
    return tableCol;
}

/*QuickSort --- Colunas*/
void QsortCol(int v[], int li, int ls){
	int j;
	if (li < ls){
		j = rearranjaCol (v,li, ls);
		QsortCol (v, li, j-1);
		QsortCol (v, j+1, ls);
	}
}

int rearranjaCol(int vect[], int li, int ls){
	int a, down, up, temp;
	a=vect[li];
	down=li;
	up=ls;
	while(down<up){
		while(vect[down]>=a && down<ls)
			down++;
		while(vect[up]<a)
			up--;
		if(down < up){
			temp=vect[down];
			vect[down]=vect[up];
			vect[up]=temp;
		}
	}
	vect[li]=vect[up];
	vect[up]=a;
	return up;
}

/*Função que serve para copiar uma tabela*/
int **copyTable(int **table, int lines, int columns){
    int i, j;
    int **tableDest;
    tableDest = (int **)malloc(lines*sizeof(int *));
    for(i=0; i<lines; i++){
        tableDest[i] = (int *)malloc(columns*sizeof(int));
        for(j=0; j<columns; j++){
            tableDest[i][j] = table[i][j];
        }
    }
    return tableDest;
}
