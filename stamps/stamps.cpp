/*
ID: liulonn1
PROG: stamps
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cmath>
#include <cstdlib>

int icmp(const void * i1, const void * i2) {
    int n1 = *((int *)(i1));
    int n2 = *((int *)(i2));
    if (n1 < n2) return -1;
    if (n1 > n2) return 1;
    return 0;
}

int main() {
    FILE * fin = fopen("stamps.in", "r");
    FILE * fout = fopen("stamps.out", "w");
    // FILE * fout = stdout;
    int k, n;

    int ret = fscanf(fin, "%d %d", &k, &n);
    assert(ret == 2);
    int * vs = new int[n];
    for (int i = 0; i < n; i++) {
        ret = fscanf(fin, "%d", &vs[i]);
        assert(ret == 1);
        assert(vs[i] <= 10000);
    }
    fclose(fin);

    qsort(vs, n, sizeof(int), icmp);

    int * buf = new int[20000];
    bzero(buf, 2 * n * sizeof(int));
    buf[0] = 0;
    for (int i = 0; i < n; i++) {
        buf[vs[i]] = 1;
    }
    int pt = 1;
    
    while (true) {
        int ind = pt % 20000;
        if (buf[ind] == 0) {
            break;
        }
        int t = buf[ind] + 1;
        if (t <= k) {
            for (int i = 0; i < n; i++) {
                int indi = (pt + vs[i]) % 20000;
                if (buf[indi] == 0 || buf[indi] > t) {
                    buf[indi] = t;
                }
            }
        }
        buf[ind] = 0;
        pt++;
        assert(pt < 200 * 10000 + 10);
    }
    
    fprintf(fout, "%d\n", pt - 1);
    fclose(fout);
    return 0;
}

