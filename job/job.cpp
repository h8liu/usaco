/*
ID: liulonn1
PROG: job
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>

int icmp(const void *a, const void *b) {
    int x = *((const int *)(a));
    int y = *((const int *)(b));
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

struct Prob {
    FILE * fin;
    FILE * fout;

    int n, m1, m2;

    int t1s[50];
    int t2s[50];

    int t1fs[30000];
    int t2fs[30000];
    
    void readIn() {
        fin = fopen("job.in", "r"); assert(fin);
        int ret = fscanf(fin, "%d %d %d", &n, &m1, &m2);
        assert(ret == 3);

        for (int i = 0; i < m1; i++) {
            ret = fscanf(fin, "%d", &t1s[i]);
            assert(ret == 1);
        }
        for (int i = 0; i < m2; i++) {
            ret = fscanf(fin, "%d", &t2s[i]);
            assert(ret == 1);
        }

        fclose(fin);
    }

    void writeOut() {
        int x = 0;

        for (int i = 0; i < m1; i++) {
            int t = t1s[i];
            int T = 0;
            for (int j = 0; j < n; j++) {
                T += t;
                t1fs[x++] = T;
            }
        }

        qsort(t1fs, x, sizeof(int), icmp);

        x = 0;
        for (int i = 0; i < m2; i++) {
            int t = t2s[i];
            int T = 0;
            for (int j = 0; j < n; j++) {
                T += t;
                t2fs[x++] = T;
            }
        }

        qsort(t2fs, x, sizeof(int), icmp);

        int Tmax = 0;
        for (int i = 0; i < n; i++) {
            int i2 = n - 1 - i;
            int T = t1fs[i] + t2fs[i2];
            if (T > Tmax) {
                Tmax = T;
            }
        }

        fout = fopen("job.out", "w"); assert(fout);
        fprintf(fout, "%d %d\n", t1fs[n-1], Tmax);
        fclose(fout);
    }
};

Prob prob;

int main() {
    prob.readIn();
    prob.writeOut();
    return 0;
}

