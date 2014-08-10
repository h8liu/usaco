/*
ID: liulonn1
PROG: shuttle
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <stdint.h>

struct Prob {
    FILE * fin;
    FILE * fout;

    int n;
    
    void readIn() {
        fin = fopen("shuttle.in", "r"); assert(fin);
        int ret = fscanf(fin, "%d", &n); assert(ret == 1);
        fclose(fin);
    }

    int count;
    void output(int i) {
        if (count % 20 > 0)
            fprintf(fout, " ");

        fprintf(fout, "%d", i);
    
        count++;
        if (count % 20 == 0)
            fprintf(fout, "\n");
    }

    void writeOut() {
        fout = fopen("shuttle.out", "w"); assert(fout);

        count = 0;
        int cur = n;
        output(cur);
        bool forward = true;
        
        for (int i = 0; i < n - 1; i++) {
            if (forward) {
                for (int j = 0; j < i+1; j++) {
                    cur += 2;
                    output(cur);
                }
                output(++cur);
                forward = false;
            } else {
                for (int j = 0; j < i+1; j++) {
                    cur -= 2;
                    output(cur);
                }
                output(--cur);
                forward = true;
            }
        }

        if (forward) {
            for (int j = 0; j < n; j++) {
                cur += 2;
                output(cur);
            }
        } else {
            for (int j = 0; j < n; j++) {
                cur -= 2;
                output(cur);
            }
        }
        forward = !forward;

        for (int i = n - 1; i >= 0; i--) {
            if (forward) {
                output(++cur);
                for (int j = 0; j < i; j++) {
                    cur += 2;
                    output(cur);
                }
                forward = false;
            } else {
                output(--cur);
                for (int j = 0; j < i; j++) {
                    cur -= 2;
                    output(cur);
                }
                forward = true;
            }
        }

        if (count % 20 != 0) fprintf(fout, "\n");

        fclose(fout);
    }
};

Prob prob;

int main() {
    prob.readIn();
    prob.writeOut();
    return 0;
}

