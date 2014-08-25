/*
ID: liulonn1
PROG: snail
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>

struct Prob {
    FILE * fin;
    FILE * fout;

    int n, b;
    char * grid;
    
    void readIn() {
        fin = fopen("snail.in", "r"); assert(fin);
        int ret = fscanf(fin, "%d %d", &n, &b); assert(ret == 2);

        char buf[64];
        grid = new char[n * n];
        memset(grid, 0, sizeof(char) * n * n);

        for (int i = 0; i < b; i++) {
            ret = fscanf(fin, "%s", buf); assert(ret == 1);
            int col = int(buf[0] - 'A');
            int row = int(atoi(&buf[1]) - 1);
            assert(0 <= row && row < n);
            assert(0 <= col && col < n);
            grid[row * n + col] = '#';
        }

        fclose(fin);
    }

    int maxTotal;

    void extend(int t) {
        if (t > maxTotal) {
            maxTotal = t;

            /*
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    char c = grid[i*n + j];
                    if (c == 0) { c = '.'; }
                    printf("%c ", c);
                }
                printf("\n");
            }
            printf("\n");
            */
        }
    }

    void go(int x, int y, int dir, int total) {
        extend(total);

        int step = 0;
        if (dir == 0) {
            // going right
            while (1) {
                int next = x + 1;
                if (next >= n || grid[next * n + y] != 0) {
                    if (step == 0) { // dead end
                        return;
                    }
                    if (next >= n || grid[next * n + y] == '#') {
                        go(x, y, 1, total+step); // try up
                        go(x, y, 3, total+step); // try down
                        break;
                    } else {
                        extend(total+step);
                        break;
                    }
                }
                
                step++;
                x++;
                grid[x * n + y] = 'x';
            }

            for (int i = 0; i < step; i++) {
                grid[x * n + y] = 0;
                x--;
            }
        } else if (dir == 1) {
            // going up
            while (1) {
                int next = y - 1;
                if (next < 0 || grid[x * n + next] != 0) {
                    if (step == 0) { // dead end
                        return;
                    }
                    if (next < 0 || grid[x * n + next] == '#') {
                        go(x, y, 0, total+step); // try right
                        go(x, y, 2, total+step); // try left
                        break;
                    } else {
                        extend(total+step);
                        break;
                    }
                }
                
                step++;
                y--;
                grid[x * n + y] = 'x';
            }

            for (int i = 0; i < step; i++) {
                grid[x * n + y] = 0;
                y++;
            }
        } else if (dir == 2) {
            // going left
            while (1) {
                int next = x - 1;
                if (next < 0 || grid[next * n + y] != 0) {
                    if (step == 0) { // dead end
                        return;
                    }
                    if (next < 0 || grid[next * n + y] == '#') {
                        go(x, y, 1, total+step); // try up
                        go(x, y, 3, total+step); // try down
                        break;
                    } else {
                        extend(total+step);
                        break;
                    }
                }
                
                step++;
                x--;
                grid[x * n + y] = 'x';
            }

            for (int i = 0; i < step; i++) {
                grid[x * n + y] = 0;
                x++;
            }
        } else if (dir == 3) {
            // going down
            while (1) {
                int next = y + 1;
                if (next >= n || grid[x * n + next] != 0) {
                    if (step == 0) { // dead end
                        return;
                    }
                    if (next >= n || grid[x * n + next] == '#') {
                        go(x, y, 0, total+step); // try right
                        go(x, y, 2, total+step); // try left
                        break;
                    } else {
                        extend(total+step);
                        break;
                    }
                }
                
                step++;
                y++;
                grid[x * n + y] = 'x';
            }

            for (int i = 0; i < step; i++) {
                grid[x * n + y] = 0;
                y--;
            }
        }
    }

    void writeOut() {
        maxTotal = 1;
        grid[0] = 'S';
        go(0, 0, 0, 1);
        go(0, 0, 3, 1);

        fout = fopen("snail.out", "w"); assert(fout);
        fprintf(fout, "%d\n", maxTotal);
        fclose(fout);
    }
};

Prob prob;

int main() {
    prob.readIn();
    prob.writeOut();
    return 0;
}

