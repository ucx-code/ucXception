#include <stdio.h>
char map[100][101];
short path[10000][2];
short nlines;
short ncols;

short checkPath(short  line, short  col, char car, short  count, short  x, short  y)
{
    if (col >= 0 && col < ncols && line >= 0 && line < nlines) {
        if (map[line][col] == car) {
            if (line == (nlines - 1)) {
                path[count][0] = line + 1;
                path[count][1] = col + 1;
                return 1;
            }else
                if (map[line][col] == '\\') {
                    if (checkPath(line + 1, col, '/', count + 1, line, col) == 1) {
                        path[count][0] = line + 1;
                        path[count][1] = col + 1;
                        return 1;
                    }
                    if (checkPath(line + 1, col + 1, '\\', count + 1, line, col) == 1) {
                        path[count][0] = line + 1;
                        path[count][1] = col + 1;
                        return 1;
                    }
                    if (checkPath(line + 1, col + 1, '|', count + 1, line, col) == 1) {
                        path[count][0] = line + 1;
                        path[count][1] = col + 1;
                        return 1;
                    }
                    if ((line != x) || (col + 1 != y)) 
                        if (checkPath(line, col + 1, '/', count + 1, line, col) == 1) {
                            path[count][0] = line + 1;
                            path[count][1] = col + 1;
                            return 1;
                        }

                    if ((line != x) || (col - 1 != y)) {
                        if (checkPath(line, col - 1, '/', count + 1, line, col) == 1) {
                            path[count][0] = line + 1;
                            path[count][1] = col + 1;
                            return 1;
                        }
                    }
                }else
                    if (map[line][col] == '/') {
                        if (checkPath(line + 1, col - 1, '|', count + 1, line, col) == 1) {
                            path[count][0] = line + 1;
                            path[count][1] = col + 1;
                            return 1;
                        }
                        if (checkPath(line + 1, col - 1, '/', count + 1, line, col) == 1) {
                            path[count][0] = line + 1;
                            path[count][1] = col + 1;
                            return 1;
                        }
                        if (checkPath(line + 1, col, '\\', count + 1, line, col) == 1) {
                            path[count][0] = line + 1;
                            path[count][1] = col + 1;
                            return 1;
                        }
                        if ((line != x) || (col + 1 != y)) 
                            if (checkPath(line, col + 1, '\\', count + 1, line, col) == 1) {
                                path[count][0] = line + 1;
                                path[count][1] = col + 1;
                                return 1;
                            }

                        if ((line != x) || (col - 1 != y)) 
                            if (checkPath(line, col - 1, '\\', count + 1, line, col) == 1) {
                                path[count][0] = line + 1;
                                path[count][1] = col + 1;
                                return 1;
                            }
                    }else
                        if (map[line][col] == '|') {
                            if (checkPath(line + 1, col - 1, '/', count + 1, line, col) == 1) {
                                path[count][0] = line + 1;
                                path[count][1] = col + 1;
                                return 1;
                            }
                            if (checkPath(line + 1, col, '|', count + 1, line, col) == 1) {
                                path[count][0] = line + 1;
                                path[count][1] = col + 1;
                                return 1;
                            }
                            if (checkPath(line + 1, col + 1, '\\', count + 1, line, col) == 1) {
                                path[count][0] = line + 1;
                                path[count][1] = col + 1;
                                return 1;
                            }
                        }

            map[line][col] = '#';
        }
    }
    return 0;
}

int main()
{
    short line = 0, col = 0, count = 0, aux, i;
    while (scanf("%hd %hd", &nlines, &ncols) != EOF){
        for (i = 0;i < nlines;i++){
            scanf("%s", map[i]);
        }
        for (col = 0;col < ncols;col++){
            if (map[0][col] == '\\') {
                if (checkPath(line + 1, col, '/', count + 1, line, col) == 1) {
                    aux = 1;
                }else
                    if (checkPath(line + 1, col + 1, '\\', count + 1, line, col) == 1) {
                        aux = 1;
                    }else
                        if (checkPath(line + 1, col + 1, '|', count + 1, line, col) == 1) {
                            aux = 1;
                        }
            }else
                if (map[0][col] == '/') {
                    if (checkPath(line + 1, col, '\\', count + 1, line, col) == 1) {
                        aux = 1;
                    }else
                        if (checkPath(line + 1, col - 1, '|', count + 1, line, col) == 1) {
                            aux = 1;
                        }else
                            if (checkPath(line + 1, col - 1, '/', count + 1, line, col) == 1) {
                                aux = 1;
                            }
                }else
                    if (map[0][col] == '|') {
                        if (checkPath(line + 1, col - 1, '/', count + 1, line, col) == 1) {
                            aux = 1;
                        }else
                            if (checkPath(line + 1, col, '|', count + 1, line, col) == 1) {
                                aux = 1;
                            }else
                                if (checkPath(line + 1, col + 1, '\\', count + 1, line, col) == 1) {
                                    aux = 1;
                                }
                    }

            if (aux == 1) {
                path[0][0] = 1;
                path[0][1] = col + 1;
                break;
            }
        }
        if (aux == 0) {
            printf("No Path!\n");
        }else{
            i = 1;
            printf("(%hd,%hd)", path[0][0], path[0][1]);
            while (path[i][0] != 0){
                printf(",(%hd,%hd)", path[i][0], path[i][1]);
                path[i][0] = 0;
                path[i][1] = 0;
                i++;
            }
            printf("\n");
            aux = 0;
        }
    }
    return 0;
}
