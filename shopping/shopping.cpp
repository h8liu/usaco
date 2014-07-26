/*
ID: liulonn1
PROG: shopping
LANG: C++
*/

#define _P "shopping"

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>

struct I {
    int c;
    int n;
};

struct S {
    int nitems;
    I items[5];
    int p;
};

struct R {
    int c;
    int k;
    int p;
};

struct S2 {
    int n[5];
    int p;
};

inline int encode(int * c, int nr) {
    int ret = 0;
    for (int i = 0; i < nr; i++) {
        ret *= 6;
        ret += c[i];
    }
    return ret;
}

inline void decode(int * c, int nr, int en) {
    for (int i = 0; i < nr; i++) {
        c[nr - 1 - i] = en % 6;
        en /= 6;
    }
}

inline int getmax(int nr) {
    int ret = 1;
    for (int i = 0; i < nr; i++) {
        ret *= 6;
    }
    return ret;
}

inline bool vleq(int * a, int * b, int n) {
    for (int i = 0; i < n; i++) {
        if (!(a[i] <= b[i])) return false;
    }
    return true;
}

inline void vadd(int * res, int * a, int * b, int n) {
    for (int i = 0; i < n; i++) {
        res[i] = a[i] + b[i];
    }
}

int result(S2 * lines, int nl, int * req, int nr) {
    assert(nr >= 0 && nr <= 5);

    int imax = getmax(nr);
    int maxsave = 0;

    int * b = new int[imax];
    for (int i = 0; i < imax; i++) {
        b[i] = -1;
    }
    b[0] = 0;

    /*
    for (int j = 0; j < nr; j++) {
        printf("%d ", req[j]);
    }
    printf("\n");
    */

    int cur[5];
    int target[5];
    for (int i = 0; i < imax; i++) {
        if (b[i] < 0) { // non reachable
            continue;
        }
        decode(cur, nr, i);
        /*
        for (int j = 0; j < nr; j++) {
            printf("%d ", cur[j]);
        }
        printf("(%d) ", i);
        printf("  %d\n", b[i]);
        */
        assert(vleq(cur, req, nr));

        for (int j = 0; j < nl; j++) {
            vadd(target, cur, lines[j].n, nr);
            if (vleq(target, req, nr)) {
                int newsave = b[i] + lines[j].p;
                if (newsave > maxsave) {
                    maxsave = newsave;
                }
                int itarget = encode(target, nr);
                /*
                printf(" -> ");
                for (int k = 0; k < nr; k++) {
                    printf("%d ", target[k]);
                }
                printf("(%d) ", itarget);
                printf("  %d\n", newsave);
                */
                
                assert(itarget > i);
                if (b[itarget] < 0 || b[itarget] < newsave) {
                    b[itarget] = newsave;
                }
            }
        }
    }

    return maxsave;
}

int main() {
    FILE * fin = fopen(_P ".in", "r");
    assert(fin);
    FILE * fout = fopen(_P ".out", "w");
    assert(fout);

    int s;
    int ret = fscanf(fin, "%d", &s);
    S * ss = new S[s];
    for (int i = 0; i < s; i++) {
        S & ts = ss[i];
        ret = fscanf(fin, "%d", &ts.nitems);
        assert(ret == 1);
        for (int j = 0; j < ts.nitems; j++) {
            ret = fscanf(fin, "%d %d", &ts.items[j].c, &ts.items[j].n);
            assert(ret == 2);
        }
        ret = fscanf(fin, "%d", &ts.p);
    }

    int b;
    ret = fscanf(fin, "%d", &b);
    R * rs = new R[b];
    int * codemap = new int[1000];
    for (int i = 0; i < 100; i++) {
        codemap[i] = -1;
    }

    for (int i = 0; i < b; i++) {
        ret = fscanf(fin, "%d %d %d", &rs[i].c, &rs[i].k, &rs[i].p);
        codemap[rs[i].c] = i;
    }
    fclose(fin);

    S2 * ss2 = new S2[s];
    int ns2 = 0;

    for (int i = 0; i < s; i++) {
        S & ts = ss[i];
        S2 & s2 = ss2[ns2];
        for (int j = 0; j < 5; j++) s2.n[j] = 0;
        s2.p = 0;
        bool useful = (ts.nitems > 0);
        
        int psum = 0;
        for (int j = 0; j < ts.nitems; j++) {
            int ind = codemap[ts.items[j].c];
            if (ind < 0) {
                useful = false; break;
            }
            psum += rs[ind].p * ts.items[j].n;
            s2.n[ind] = ts.items[j].n;
        }

        if (useful && psum >= ts.p) {
            s2.p = psum - ts.p; // savings
            ns2++;
        }
    }
    delete [] ss;
    
    int * req = new int[b];
    for (int i = 0; i < b; i++) {
        req[i] = rs[i].k;
    }
    int origsum = 0;
    for (int i = 0; i < b; i++) {
        origsum += rs[i].k * rs[i].p;
    }
    delete [] rs;

    /*
    for (int i = 0; i < ns2; i++) {
        for (int j = 0; j < b; j++) {
            printf("%d ", ss2[i].n[j]);
        }
        printf("   %d", ss2[i].p);
        printf("\n");
    }
    */

    int res = result(ss2, ns2, req, b);
    // printf("%d %d\n", origsum, res);
    fprintf(fout, "%d\n", origsum - res);

    fclose(fout);
    return 0;
}

