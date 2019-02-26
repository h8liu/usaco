/*
ID: liulonn1
PROG: hidden
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>

int * order;
int stride = 1;
int len = 0;

int pcmp(const void * x, const void * y) {
    int i1 = *(const int *)(x);
    int i2 = *(const int *)(y);
    
    if (order[i1] < order[i2]) { return -1; }
    if (order[i1] > order[i2]) { return 1; }

    int ii1 = (i1 + stride) % len;
    int ii2 = (i2 + stride) % len;

    if (order[ii1] < order[ii2]) { return -1; }
    if (order[ii1] > order[ii2]) { return 1; }
    
    if (i1 < i2) { return -1; }
    if (i1 > i2) { return 1; }
    return 0;
}

int pcmp2(const void * x, const void * y) {
    int i1 = *(const int *)(x);
    int i2 = *(const int *)(y);
    
    if (order[i1] < order[i2]) { return -1; }
    if (order[i1] > order[i2]) { return 1; }

    i1 = (i1 + stride) % len;
    i2 = (i2 + stride) % len;

    if (order[i1] < order[i2]) { return -1; }
    if (order[i1] > order[i2]) { return 1; }
    
    return 0;
}

struct Prob {
    FILE * fin;
    FILE * fout;

    char buf[100000];
    
    void readIn() {
        fin = fopen("hidden.in", "r"); assert(fin);
        int ret = fscanf(fin, "%d", &len); assert(ret == 1);
        
        int cnt = 0;
        char * pt = buf;
        while (cnt < len) {
            ret = fscanf(fin, "%s", pt);
            assert(ret == 1);
            int n = strlen(pt);
            cnt += int(n);
            assert(cnt <= len);
            pt = &buf[cnt];
        }

        fclose(fin);
    }

    int cand1[100000];
    int cand2[100000];
    int od[100000];

    void writeOut() {
        for (int i = 0; i < len; i++) {
            cand1[i] = int(buf[i] - 'a');
        }
        order = cand1;
        int * order2 = cand2;
        stride = 1;

        for (int i = 0; i < len; i++) {
            od[i] = i;
        }

        while (1) {
            /*
            for (int i = 0; i < len; i++) {
                printf("%d:%d ", od[i], order[od[i]]);
            }
            printf("\n");
            printf("\n");
            */

            qsort(od, len, sizeof(int), pcmp);
 
            // compute new order
            int cnt = 0;
            order2[od[0]] = cnt;

            for (int i = 1; i < len; i++) {
                int cmp = pcmp2(&od[i-1], &od[i]);
                if (cmp == -1) {
                    cnt++;
                } else {
                    assert(cmp == 0);
                }
                order2[od[i]] = cnt;
            }

            // swap order
            int * t = order;
            order = order2;
            order2 = t;
            
            if (stride >= len) {
                break;
            }

            stride *= 2;
        }

        fout = fopen("hidden.out", "w"); assert(fout);
        fprintf(fout, "%d\n", od[0]);
        fclose(fout);
    }
};

Prob prob;

int main() {
    prob.readIn();
    prob.writeOut();
    return 0;
}

