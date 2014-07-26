/*
ID: liulonn1
PROG: rockers
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <vector>
using std::vector;

struct State {
    int ts[20];
};

struct Prob {
    FILE * fin;
    FILE * fout;

    int n; // # songs
    int m; // # disks
    int t; // time len of each disk
    int * ts;

    int maxsongs;

    void readIn() {
        fin = fopen("rockers.in", "r"); assert(fin);
        int ret;

        ret = fscanf(fin, "%d %d %d", &n, &t, &m);
        ts = new int[n];

        for (int i = 0; i < n; i++) {
            ret = fscanf(fin, "%d", &ts[i]);
            assert(ret == 1);
        }
    
        fclose(fin);
    }

    void writeOut() {
        fout = fopen("rockers.out", "w"); assert(fout);
        
        maxsongs = 0;
        int ncomb = (0x1 << n);
        
        for (int i = 0; i < ncomb; i++) {
            int ndisks = 0;
            int tcurrent = 0;
            int bitmap = i;
            int nsongs = 0;
            for (int j = 0; j < n; j++) {
                if (bitmap & 0x1) {
                    int tthis = ts[j];
                    if (tcurrent + tthis <= t) {
                        tcurrent += tthis;
                        nsongs++;
                    } else if (tthis <= t) {
                        ndisks++;
                        if (ndisks == m) {
                            break;
                        }
                        tcurrent = tthis;
                        nsongs++;
                    }
                    // printf("x");
                } else {
                    // printf(".");
                }
                bitmap >>= 1;
            }
            // printf(" %d\n", nsongs);

            if (nsongs > maxsongs) {
                maxsongs = nsongs;
            }
        }


        fprintf(fout, "%d\n", maxsongs);
        fclose(fout);
    }
};

int main() {
    Prob prob;
    prob.readIn();
    prob.writeOut();
    return 0;
}

