/*
ID: liulonn1
PROG: bigbrn
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>

struct Pos {
    int x, y;
};

struct Prob {
    FILE * fin;
    FILE * fout;
    int n;
    int t;
    int last[1000][1000];
    
    void readIn() {
        fin = fopen("bigbrn.in", "r"); assert(fin);
        int ret = fscanf(fin, "%d %d", &n, &t); assert(ret == 2);
        int x, y;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                last[i][j] = -1;
            }
        }

        for (int i = 0; i < t; i++) {
            ret = fscanf(fin, "%d %d", &x, &y); assert(ret == 2);
            x--;
            y--;
         
            last[x][y] = x;
        }

        fclose(fin);

        int fill;
        for (int i = 0; i < n; i++) {
            fill = n;
            for (int j = n-1; j >= 0; j--) {
                if (last[j][i] < 0) {
                    last[j][i] = fill;
                } else {
                    fill = last[j][i];
                }
            }
        }
    }

    void writeOut() {
        /*
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                printf("%d ", last[i][j]);
            }
            printf("\n");
        }
        */

        int res = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                int max = n - j;

                for (int col = j; col < n; col++) {
                    int r = col - j;
                    int next = last[i][col];
                    int r2 = next - i;
                    if (r2 <= r) {
                        if (max > r) { max = r; }
                        break;
                    } else {
                        if (max > r2) { max = r2; }
                    }
                }

                // printf("%d %d: %d\n", i, j, max);

                if (max > res && max != n) {
                    res = max;
                }
            }
        }

        fout = fopen("bigbrn.out", "w"); assert(fout);
        fprintf(fout, "%d\n", res);
        fclose(fout);
    }
};

Prob prob;

int main() {
    prob.readIn();
    prob.writeOut();
    return 0;
}

