/*
ID: liulonn1
PROG: combo
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>

struct Prob {
    FILE * fin;
    FILE * fout;

    int n;
    int a1, a2, a3;
    int b1, b2, b3;

    int c1, c2, c3;
    
    int absd(int x, int y) {
        int d = x - y;
        if (d < 0) { d = -d; }
        int d2 = n - d;
        if (d2 < d) {
            d = d2;
        }
        return d;
    }

    void readIn() {
        fin = fopen("combo.in", "r"); assert(fin);
        int ret = fscanf(fin, "%d", &n); assert(ret == 1);
        ret = fscanf(fin, "%d %d %d", &a1, &a2, &a3); assert(ret == 3);
        ret = fscanf(fin, "%d %d %d", &b1, &b2, &b3); assert(ret == 3);

        fclose(fin);
    }

    void writeOut() {
        c1 = c2 = c3 = 0;

        for (int i = 0; i < n; i++) {
            if (absd(i, a1) <= 2 && absd(i, b1) <= 2) {
                c1++;
            }
            if (absd(i, a2) <= 2 && absd(i, b2) <= 2) {
                c2++;
            }
            if (absd(i, a3) <= 2 && absd(i, b3) <= 2) {
                c3++;
            }
        }

        int t = n;
        if (t > 5) { t = 5; }
        int res = t * t * t * 2 - c1 * c2 * c3;

        fout = fopen("combo.out", "w"); assert(fout);
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

