/*
ID: liulonn1
PROG: milk4
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <stdint.h>

int icmp(const void *a, const void *b) {
    int x = *((int *)(a));
    int y = *((int *)(b));
    if (x > y) { return -1; }
    if (x < y) { return 1; }
    return 0;
}

struct Bm {
    uint64_t hi;
    uint64_t lo;
    uint32_t cnt;
    bool valid;

    void clear() { hi = 0; lo = 0; cnt = 0; }
    void set(int i) {
        if (!get(i)) {
            cnt++;
        }
        if (i >= 64) { 
            hi |= (((uint64_t)(0x1)) << (i - 64)); 
        } else { 
            lo |= (((uint64_t)(0x1)) << i);
        }
    }
    bool get(int i) {
        if (i >= 64) {
            return (hi & (((uint64_t)(0x1)) << (i - 64))) != 0;
        } else {
            return (lo & (((uint64_t)(0x1)) << i)) != 0;
        }
    }
    bool empty() {
        return hi == 0 && lo == 0;
    }
    bool less(Bm & other) {
        if (cnt < other.cnt) { return true; }
        if (cnt > other.cnt) { return false; }
        if (hi > other.hi) { return true; }
        if (hi < other.hi) { return false; }
        if (lo > other.lo) { return true; }
        return false;
    }
};

struct Prob {
    FILE * fin;
    FILE * fout;

    int q, p;
    int pv[100];
    Bm bm[20010];
    
    void readIn() {
        fin = fopen("milk4.in", "r"); assert(fin);
        int ret = fscanf(fin, "%d", &q); assert(ret == 1);
        ret = fscanf(fin, "%d", &p); assert(ret == 1);

        for (int i = 0; i < p; i++) {
            ret = fscanf(fin, "%d", &pv[i]); assert(ret == 1);
        }
        fclose(fin);
    }

    void writeOut() {
        qsort(pv, p, sizeof(int), icmp);

        memset(bm, 0, sizeof(Bm) * q);
        
        bm[0].valid = true;

        for (int i = 0; i <= q; i++) {
            Bm best;
            Bm tmp;
            best = bm[i];
            tmp.valid = false;

            for (int j = p-1; j >= 0; j--) {
                int v = pv[j];
                if (v > i) {
                    continue;
                }
                for (int k = i - v; k >= 0; k -= v) {
                    if (!bm[k].valid) {
                        continue;
                    }
                    tmp = bm[k];
                    tmp.set(j);
                    if (!best.valid || tmp.less(best)) {
                        best = tmp;
                    }
                }
            }

            if (best.valid) {
                bm[i] = best;
                /*
                printf("%d:", i);
                for (int i = p-1; i >= 0; i--) {
                    if (best.get(i)) {
                        printf(" %d", pv[i]);
                    }
                }
                printf("\n");
                */
            } else {
                // printf("// %d\n", i);
            }
        }

        fout = fopen("milk4.out", "w"); assert(fout);

        assert(bm[q].valid && bm[q].cnt > 0);
        fprintf(fout, "%d", bm[q].cnt);
        for (int i = p-1; i >= 0; i--) {
            if (bm[q].get(i)) {
                fprintf(fout, " %d", pv[i]);
            }
        }
        fprintf(fout, "\n");

        fclose(fout);
    }
};

Prob prob;

int main() {
    prob.readIn();
    prob.writeOut();
    return 0;
}

