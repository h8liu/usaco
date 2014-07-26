/*
ID: liulonn1
PROG: stall4
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>

struct Prob {
    FILE * fin;
    FILE * fout;
    
    int n, m;

    bool d[300][300];
    int destFor[300];
    bool searched[300];
    
    void readIn() {
        fin = fopen("stall4.in", "r"); assert(fin);

        int read = fscanf(fin, "%d %d", &n, &m);
        assert(read == 2);

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                d[i][j] = false;
            }
        }

        for (int i = 0; i < n; i++) {
            destFor[i] = -1;
        }

        for (int i = 0; i < n; i++) {
            int k;
            read = fscanf(fin, "%d", &k);
            assert(read == 1);

            for (int j = 0; j < k; j++) {
                int t;
                read = fscanf(fin, "%d", &t);
                assert(read == 1);
                t--;
                
                d[i][t] = true;
            }
        }

        fclose(fin);
    }

    bool findFor(int dest) {
        for (int i = 0; i < n; i++) {
            if (!d[i][dest]) { continue; }
            if (searched[i]) { continue; }
            searched[i] = true;

            int extend = destFor[i];
            if (extend == -1 || findFor(extend)) {
                destFor[i] = dest;
                return true;
            }
        }
        return false;
    }

    int match() {
        int ret = 0;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                searched[j] = false;
            }
            if (findFor(i)) {
                ret++;
            }
        }
        return ret;
    }

    void writeOut() {
        int res = match();

        fout = fopen("stall4.out", "w"); assert(fout);
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

