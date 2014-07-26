/*
ID: liulonn1
PROG: msquare
LANG: C++
*/

#define _P "msquare"

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>

struct D {
    int depth;
    int p[8];
    int from;
    char by;
};

int ima[8] = {8, 7, 6, 5, 4, 3, 2, 1};
int imb[8] = {4, 1, 2, 3, 6, 7, 8, 5};
int imc[8] = {1, 7, 2, 4, 5, 3, 6, 8};

void mv(D & f, D & t, int * im) {
    t.depth = f.depth + 1;
    
    for (int i = 0; i < 8; i++) {
        t.p[i] = f.p[im[i]-1];
    }
}

int index(D & d) {
    int ret = 0;
    int r[8];
    for (int i = 0; i < 8; i++) {
        int c = 0;
        for (int j = i + 1; j < 8; j++) {
            if (d.p[j] < d.p[i]) {
                c++;
            }
        }
        r[d.p[i]] = c;
    }
    
    for (int i = 0; i < 8; i++) {
        ret *= (i + 1);
        ret += r[i];
    }
    assert(ret >= 0 && ret < 40320);
    return ret;
}

void ini(D & d) {
    d.depth = 0;
    for (int i = 0; i < 8; i++) {
        d.p[i] = i;
    }
}

inline void bset(unsigned char * bs, int i) {
    int n = i / 8;
    int b = i % 8;
    bs[n] |= ((unsigned char)(0x1) << b);
}

inline bool bget(unsigned char * bs, int i) {
    int n = i / 8;
    int b = i % 8;
    return (bs[n] & ((unsigned char)(0x1) << b)) != 0;
}

int main() {
    FILE * fin = fopen(_P ".in", "r");
    assert(fin);
    FILE * fout = fopen(_P ".out", "w");
    // FILE * fout = stdout;
    
    unsigned char * occ = new unsigned char[5040]; // 8 ** 8
    assert(occ);
    bzero(occ, 5040);
    
    D tar;
    tar.depth = 0;
    int ret;
    for (int i = 0; i < 8; i++) {
        ret = fscanf(fin, "%d", &(tar.p[i]));
        assert(ret == 1);
        tar.p[i]--;
    }
    ret = -1;
    int tarind = index(tar);
    fclose(fin);
    
    D * d = new D[40320];
    assert(d);
    ini(d[0]);
    int i = index(d[0]);
    if (tarind == i) {
        ret = 0;
    } else {
        int tail = 0;
        int head = 1;
        bset(occ, i);
        d[0].from = -1;
        d[0].by = ' ';

        D * pnew = &d[head];

        while (tail < head) {
            D & dt = d[tail];

            mv(dt, *pnew, ima);
            pnew->by = 'A';
            pnew->from = tail;
            i = index(*pnew);
            if (i == tarind) {
                ret = head;
                break;
            } else if (!bget(occ, i)) {
                bset(occ, i);
                pnew = &d[++head];
                pnew->by = '.';
            }

            mv(dt, *pnew, imb);
            pnew->by = 'B';
            pnew->from = tail;
            i = index(*pnew);
            if (i == tarind) {
                ret = head;
                break;
            } else if (!bget(occ, i)) {
                bset(occ, i);
                pnew = &d[++head];
                pnew->by = '.';
            }

            mv(dt, *pnew, imc);
            pnew->by = 'C';
            pnew->from = tail;
            i = index(*pnew);
            if (i == tarind) {
                ret = head;
                break;
            } else if (!bget(occ, i)) {
                bset(occ, i);
                pnew = &d[++head];
                pnew->by = '.';
            }
            
            tail++;
        }
        // printf("%d %d\n", tail, head);
    }

    assert(ret >= 0);

    int n = d[ret].depth;
    fprintf(fout, "%d\n", n);
    char * outbuf = new char[n + 1];
    outbuf[n] = '\0';

    while (n > 0) {
        D & rd = d[ret];
        if (rd.from < 0) break;
        outbuf[--n] = rd.by; 
        ret = rd.from;
    }
    fprintf(fout, "%s\n", outbuf);
    
    fclose(fout);
    return 0;
}

