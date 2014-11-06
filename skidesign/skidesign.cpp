/*
ID: liulonn1
PROG: skidesign
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

    int hs[1000];
    
    void readIn() {
        fin = fopen("skidesign.in", "r"); assert(fin);
        int ret = fscanf(fin, "%d", &n); assert(ret == 1);
        for (int i = 0; i < n; i++) {
            ret = fscanf(fin, "%d", &hs[i]); assert(ret == 1);
        }
        fclose(fin);
    }

    void writeOut() {
        
        int minSum = -1;

        for (int low = 0; low <= 100 - 17; low++) {
            int high = low + 17;
            int sum = 0;
            for (int i = 0; i < n; i++) {
                if (hs[i] < low) {
                    int d = low - hs[i];
                    sum += d*d;
                } else if (hs[i] > high) {
                    int d = hs[i] - high;
                    sum += d*d;
                }
            }

            // printf("low=%d, high=%d, pay=%d\n", low, high, sum);

            if (minSum < 0 || sum < minSum) {
                minSum = sum;
            }
        }

        fout = fopen("skidesign.out", "w"); assert(fout);
        fprintf(fout, "%d\n", minSum);
        fclose(fout);
    }
};

Prob prob;

int main() {
    prob.readIn();
    prob.writeOut();
    return 0;
}

