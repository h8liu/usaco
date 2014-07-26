/*
ID: liulonn1
PROG: camelot
LANG: C++
*/

#define _P "camelot"

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>

FILE * fin;
FILE * fout;
int rmax, cmax;
int ret;

struct Coord { int r, c; int d; };
Coord king;
Coord knights[900];
int nknight;
#define MAXIND (32 * 32)
int cdist[MAXIND * MAXIND];

inline bool readpos(FILE * fin, Coord * p) {
    char sbuf[8];
    ret = fscanf(fin, "%s %d", sbuf, &p->r);
    if (ret != 2) return false;
    char c = sbuf[0];

    p->r -= 1;
    
    assert(c >= 'A' && c <= 'Z');
    p->c = c - 'A';

    return true;
}

inline void printpos(Coord * p) {
    printf("%d, %d\n", p->c, p->r);
}

inline int indexof(int r, int c) {
    return (c << 5) + r;
}

inline int indexof(Coord * p) {
    return indexof(p->r, p->c);
}

Coord trace[MAXIND];
bool touched[MAXIND];
Coord steps[8] = { 
    {1, 2, 0},
    {-1, 2, 0},
    {1, -2, 0},
    {-1, -2, 0},
    {2, 1, 0},
    {2, -1, 0},
    {-2, 1, 0},
    {-2, -1, 0},
};

Coord ksteps[8] = {
    {1, 1, 0},
    {1, 0, 0},
    {1, -1, 0},
    {0, 1, 0},
    {0, -1, 0},
    {-1, 1, 0},
    {-1, 0, 0},
    {-1, -1, 0},
};

inline int iabs(int i) { return (i >= 0)?i:-i; }
inline int imax(int a, int b) { return (a > b)?a:b; }

void computeSingle(int * dist, int r, int c) {
    memset(dist, 0xff, sizeof(int) * MAXIND); // clear the distances
    memset(touched, 0, sizeof(bool) * MAXIND);
    
    int head = 0;
    int tail = 1;
    trace[0].r = r; trace[0].c = c;
    trace[0].d = 0;
    int indstart = indexof(r, c);
    touched[indstart] = true;
    dist[indstart] = 0;
    
    while (head < tail) {
        int rthis = trace[head].r;
        int cthis = trace[head].c;
        int dthis = trace[head].d;

        for (int i = 0; i < 8; i++) {
            int rthat = rthis + steps[i].r;
            int cthat = cthis + steps[i].c;
            
            if (rthat < 0 || rthat >= rmax) continue;
            if (cthat < 0 || cthat >= cmax) continue;
            int ind = indexof(rthat, cthat);
            if (touched[ind]) continue;
            // pushing back
            int dthat = dthis + 1; // min dist
            touched[ind] = true;
            dist[ind] = dthat; // saving min dist
            trace[tail].r = rthat;
            trace[tail].c = cthat;
            trace[tail].d = dthat;
            tail++;
        }
        head++;
    }

}

void computeAll() {
    for (int r = 0; r < rmax; r++) {
        for (int c = 0; c < cmax; c++) {
            computeSingle(&cdist[indexof(r, c) * MAXIND], r, c);
        }
    }
}

void computeAll2() {
    // (rmax * cmax) ** 3
    memset(cdist, 0x3f, sizeof(int) * MAXIND * MAXIND);

    // init
    for (int r1 = 0; r1 < rmax; r1++) {
        for (int c1 = 0; c1 < cmax; c1++) {
            int ind1 = indexof(r1, c1);
            cdist[ind1 * MAXIND + ind1] = 0;
            for (int i = 0; i < 8; i++) {
                int r2 = r1 + steps[i].r;
                int c2 = c1 + steps[i].c;
                if (r2 < 0 || r2 >= rmax) continue;
                if (c2 < 0 || c2 >= cmax) continue;
                int ind2 = indexof(r2, c2);

                cdist[ind1 * MAXIND + ind2] = 1;
                cdist[ind2 * MAXIND + ind1] = 1;
            }
        }
    }

    for (int r1 = 0; r1 < rmax; r1++) {
        for (int c1 = 0; c1 < cmax; c1++) {
            int k = indexof(r1, c1);
            for (int r2 = 0; r2 < rmax; r2++) {
                for (int c2 = 0; c2 < cmax; c2++) {
                    int i = indexof(r2, c2);
                    for (int r3 = 0; r3 < rmax; r3++) {
                        for (int c3 = 0; c3 < cmax; c3++) {
                            int j = indexof(r3, c3);
                            int dik = cdist[i * MAXIND + k];
                            int dkj = cdist[k * MAXIND + j];
                            int dij = cdist[i * MAXIND + j];
                            if (dik + dkj < dij) {
                                cdist[i * MAXIND + j] = dik + dkj;
                                cdist[j * MAXIND + i] = dik + dkj;
                            }
                        }
                    }
                }
            }
        }
    }
}

inline int minsteps(int ind1, int ind2) {
    return cdist[ind1 * MAXIND + ind2];
}

int main() {
    fin = fopen(_P ".in", "r"); assert(fin);
    fout = fopen(_P ".out", "w"); assert(fout);
    
    // read input
    ret = fscanf(fin, "%d %d", &rmax, &cmax);
    assert(ret == 2);
    
    readpos(fin, &king);

    nknight = 0;
    while (readpos(fin, &knights[nknight])) {
        nknight++;
    }

    /*
    printpos(&king);
    for (int i = 0; i < nknight; i++) {
        printpos(&knights[i]);
    }
    */
    fclose(fin);

    // compute the min map
    computeAll();
    
    int mintotal = -1;
    int rking = king.r;
    int cking = king.c;

    for (int rgath = 0; rgath < rmax; rgath++) {
        for (int cgath = 0; cgath < cmax; cgath++) {
            int minkdelta = imax(iabs(rking - rgath), 
                    iabs(cking - cgath));
            int indgath = indexof(rgath, cgath);
            int total = 0;
            bool unreachable = false;
            
            for (int k = 0; k < nknight; k++) {
                int indknight = indexof(&knights[k]);
                int ms = minsteps(indgath, indknight);
                if (ms < 0) {
                    unreachable = true;
                    break;
                }
                knights[k].d = minsteps(indgath, indknight);
                total += knights[k].d;
            }

            if (unreachable) continue; // knight unreachable
            
            memset(touched, 0, sizeof(bool) * MAXIND);
            int head = 0;
            int tail = 1;
            trace[0].r = rking; trace[0].c = cking;
            trace[0].d = 0;
            
            while (head < tail) {
                int dthis = trace[head].d;
                if (dthis >= minkdelta) break;
                int rthis = trace[head].r;
                int cthis = trace[head].c;
                
                int indpick = indexof(rthis, cthis);
                int deltabase = minsteps(indgath, indpick);
                if (deltabase >= 0) { // reachable
                    int mindel = 1000;
                    for (int k = 0; k < nknight; k++) {
                        int indknight = indexof(&knights[k]);
                        int ms = minsteps(indpick, indknight);
                        if (ms < 0) continue;
                        int delta = deltabase + ms;
                        assert(delta >= knights[k].d);
                        delta -= knights[k].d;
                        if (delta < mindel) {
                            mindel = delta;
                        }
                        if (mindel == 0) break; // drive by already
                    }

                    if (nknight > 0) { // have knights
                        int dmin = dthis + mindel;
                        if (dmin < minkdelta) {
                            minkdelta = dmin;
                        }
                    }
                }

                for (int i = 0; i < 8; i++) {
                    int rthat = rthis + ksteps[i].r;
                    int cthat = cthis + ksteps[i].c;
                    if (rthat < 0 || rthat >= rmax) continue;
                    if (cthat < 0 || cthat >= cmax) continue;

                    int ind = indexof(rthat, cthat);
                    if (touched[ind]) continue;
                    int dthat = dthis + 1;
                    touched[ind] = true;
                    trace[tail].r = rthat;
                    trace[tail].c = cthat;
                    trace[tail].d = dthat;
                    tail++;
                }
                head++;
            }

            total += minkdelta;

            if (mintotal < 0 || total < mintotal) {
                mintotal = total;
            }
        }
    }

    assert(mintotal >= 0);
    fprintf(fout, "%d\n", mintotal);

    return 0;
}

