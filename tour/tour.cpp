/*
ID: liulonn1
PROG: tour
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstring>

#include <vector>
using std::vector;

struct City {
    vector<int> w;
    vector<int> e;
};

struct Prob {
    FILE * fin;
    FILE * fout;

    int n, v;
    char cnames[100][32];
    City cs[100];

    int states[100][100];

    int cid(char *c) {
        for (int i = 0; i < n; i++) {
            if (strcmp(cnames[i], c) == 0) {
                return i;
            }
        }
        return -1;
    }
    
    void readIn() {
        fin = fopen("tour.in", "r"); assert(fin);
        int ret = fscanf(fin, "%d %d", &n, &v); assert(ret == 2);
        assert(n <= 100);
        for (int i = 0; i < n; i++) {
            ret = fscanf(fin, "%s", cnames[i]);
            assert(ret == 1);
        }

        char c1[32], c2[32];
        for (int i = 0; i < v; i++) {
            ret = fscanf(fin, "%s %s", c1, c2); assert(ret == 2);
            int id1 = cid(c1);
            int id2 = cid(c2);
            assert(id1 >= 0);
            assert(id2 >= 0);
            assert(id1 != id2);
            if (id1 < id2) {
                cs[id1].e.push_back(id2);
                cs[id2].w.push_back(id1);
            } else {
                cs[id1].w.push_back(id2);
                cs[id2].e.push_back(id1);
            }
        }

        fclose(fin);
    }

    void writeOut() {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                states[i][j] = 0;
            }
        }
        states[0][0] = 1;

        for (int i = 1; i < n; i++) {
            for (int j = 0; j < i; j++) {
                City & c1 = cs[i];
                City & c2 = cs[j];

                int max = 0;
                
                for (vector<int>::iterator it = c1.w.begin();
                        it != c1.w.end(); it++) {
                    int cid = *it;
                    if (cid > j) {
                        continue;
                    }
                    if (states[cid][j] > 0) {
                        int v = states[cid][j] + 1;
                        if (v > max) { max = v; }
                    }
                }

                for (vector<int>::iterator it = c2.w.begin();
                        it != c2.w.end(); it++) {
                    int cid = *it;
                    if (states[i][cid] > 0) {
                        int v = states[i][cid] + 1;
                        if (v > max) { max = v; }
                    }
                }

                states[i][j] = max;
                states[j][i] = max;
            }
        }

        int last = n-1;
        int max = 1;
        City & c = cs[n-1];
        for (vector<int>::iterator it = c.w.begin(); 
                it != c.w.end(); it++) {
            int cid = *it;
            int v = states[cid][last];
            if (v > max) { max = v; }
            v = states[last][cid];
            if (v > max) { max = v; }
        }

        fout = fopen("tour.out", "w"); assert(fout);
        fprintf(fout, "%d\n", max);
        fclose(fout);
    }
};

Prob prob;

int main() {
    prob.readIn();
    prob.writeOut();
    return 0;
}

