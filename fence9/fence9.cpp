/*
ID: liulonn1
PROG: fence9
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>

struct Prob {
    FILE * fin;
    FILE * fout;
    int n, m, p;
    
    void readIn() {
        fin = fopen("fence9.in", "r"); assert(fin);
        int ret = fscanf(fin, "%d %d %d", &n, &m, &p);
        assert(ret == 3);
        fclose(fin);
    }

    void writeOut() {
        fout = fopen("fence9.out", "w"); assert(fout);

        int total = 0;

        double slope1 = (double)(n) / m;
        double slope2 = (double)(p - n) / m;
        for (int y = 1; y < m; y++) {
            double xleft = slope1 * y;
            double xright = p - slope2 * y;
            int ileft = (int)(xleft) - 1;
            int iright = (int)(xright) + 1;

            while (ileft * m <= n * y) {
                ileft++;
            }

            while ((iright - p) * m >= (n - p) * y) {
                iright--;
            }
            
            int line = iright - ileft + 1;
            if (line > 0) total += line;
        }

        fprintf(fout, "%d\n", total);

        fclose(fout);
    }
};

int main() {
    Prob * prob = new Prob();
    prob->readIn();
    prob->writeOut();
    delete prob;
    return 0;
}

