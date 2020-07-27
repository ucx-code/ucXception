#include <stdio.h>
int n;
int counter;
int queens[12];

void putQueen(int col)
{
    int i;
    for (i = col - 1;i >= 0;i--){
        if (queens[col] == queens[i] || queens[i] + (col - i) == queens[col] || queens[i] - (col - i) == queens[col]) {
            return;
        }
    }
    if (col + 1 == n) {
        counter++;
        return;
    }
    for (i = 0;i < n;i++){
        queens[col + 1] = i;
        putQueen(col + 1);
    }
}

int main()
{
    while (scanf("%d", &n) != EOF){
        counter = 0;
        putQueen(-1);
        printf("%d\n", counter);
    }
    return 0;
}
