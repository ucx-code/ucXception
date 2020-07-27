#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char connections[35][35];
char printers[35];
int best = 36;

int checkAll(int nr){
    
    int count = 0, i, j;
    int best1 = 0;
    char aux[nr];
    
    memset(aux, '*', nr*sizeof(char));
    
    for(i=0; i<nr && count<nr; i++){
        if(printers[i] == '1'){
            best1++;
            if(aux[i] != '1'){
                aux[i] = '1';
                count++;
            }
            for(j=0; j<i; j++){
                if(connections[j][i] == '1' && aux[j] != '1'){
                    aux[j] = '1';
                    count++;
                }
            }
            
            for(j=i+1; j<nr; j++){
                if(connections[i][j] == '1'  && aux[j] != '1'){
                    aux[j] = '1';
                    count++;
                }
            }
        }
    }
    
    /*for(i=0; i<nr; i++){
        if(aux[i] == '1'){
            count++;
        }
    }*/
    
    if(count<nr){
        return -1;
    }
    
    /*if(best1 == 5){
     for(i=0; i<nr; i++){
     printf("%c ", printers[i]);
     }
     printf("\nCOUNT->%d\n", count);
     for(i=0; i<nr; i++){
     printf("%c ", aux[i]);
     }
     printf("\n");
     }
     
     */
    if(best1 < best && best1 != 0){
        best = best1;
    }
    
    
    return 0;
}

int calc(int n, int nr, int b){
    
    int i;
    
    if(b>best){
        return 0;
    }
    
    if(checkAll(nr)==0){
        return 0;
    }
    
    for(i=n; i<nr; i++){
        if(printers[i]=='#'){
            printers[i] = '1';
            calc(i, nr, b+1);
            printers[i]='#';
        }
    }
    
    return -1;
}

int main(int argc, const char * argv[])
{
    int nr, i, j;
    
    scanf("%d", &nr);
    while(scanf("%d %d", &i, &j) == 2){
        connections[i-1][j-1] = '1';
    }
    
    memset(printers, '#', nr*sizeof(char));
    
    calc(0, nr, 1);
    
    printf("%d\n", best);
    
    return 0;
}
