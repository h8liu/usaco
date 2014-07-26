/*
ID: liulonn1
PROG: fence8
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>

int icmp(const void * a, const void * b) {
    return (*(int*)a - *(int*)b);
}

int icmpn(const void * a, const void * b) {
    return -(*(int*)a - *(int*)b);
}

inline int imin(int a, int b) {
    return (a < b)?a:b;
}

inline int imax(int a, int b) {
    return (a > b)?a:b;
}

struct Prob {
    FILE * fin;
    FILE * fout;
    int n;
    int b[50]; // boards
    int r;
    int rail[1024];
    bool railused[1024];
    int bwasted[1024];

    int bleft[50];
    int bused[50];
    int maxput;
    
    void readIn() {
        fin = fopen("fence8.in", "r"); assert(fin);
        int ret = fscanf(fin, "%d", &n); assert(ret == 1);
        for (int i = 0; i < n; i++) {
            ret = fscanf(fin, "%d", &b[i]);
            assert(ret == 1);
        }

        ret = fscanf(fin, "%d", &r); assert(ret == 1);
        for (int i = 0; i < r; i++) {
            ret = fscanf(fin, "%d", &rail[i]);
            assert(ret == 1);
        }
        
        fclose(fin);
    }

    int optimal;
    int wasted;

    void tryput(int i, int bthis, int rstart, int wasted) {
        if (i > maxput) maxput = i;
        if (maxput == optimal) return;
        if (wasted > bwasted[maxput + 1]) return;

        int rlast = 0;
        bool tried = false;
        for (int j = rstart; j < r; j++) {
            if (railused[j]) continue; // already used
            int rlen = rail[j];
            if (rlen == rlast) continue; // same length only try once
            rlast = rlen;

            if (rlen > b[bthis] - bused[bthis]) continue; // space left not enough
            if (bthis > 0 && rlen + bused[bthis] > b[bthis - 1]) continue;

            railused[j] = true;
            bused[bthis] += rlen;
            tryput(i + 1, bthis, j + 1, wasted);
            tried = true;
            bused[bthis] -= rlen;
            railused[j] = false;
        }

        if (!tried) {
            if (bthis + 1 < n) {
                tryput(i, bthis + 1, 0, wasted + b[bthis] - bused[bthis]);
            } 
        } 
    }

    void writeOut() {
        qsort(b, n, sizeof(int), icmpn);
        qsort(rail, r, sizeof(int), icmp);
    
        int bsum = 0;
        for (int i = 0; i < n; i++) {
            bleft[i] = b[i];
            bused[i] = 0;
            bsum += b[i];
        }

        bwasted[0] = bsum;

        optimal = 0;
        for (int i = 0; i < r; i++) {
            if (bsum >= rail[i]) {
                bsum -= rail[i];
                optimal++;
                bwasted[optimal] = bsum;
            } else {
                break;
            }
        }
        r = optimal;

        qsort(rail, r, sizeof(int), icmpn);

        for (int i = 0; i < r; i++) {
            railused[i] = false;
        }
    
        maxput = 0;
        
        tryput(0, 0, 0, 0);

        
        fout = fopen("fence8.out", "w"); assert(fout);
        fprintf(fout, "%d\n", maxput);
        fclose(fout);
    }
};

Prob prob;

int main() {
    prob.readIn();
    prob.writeOut();
    return 0;
}

