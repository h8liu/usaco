/*
ID: liulonn1
PROG: wormhole
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>

struct Hole {
    int x, y;
    int id;
    Hole *last;
    Hole *next;
    Hole *pairing;
    bool outChecked;
};

static int hcmp(const void *x, const void *y) {
    const Hole *a = (const Hole *)(x);
    const Hole *b = (const Hole *)(y);

    if (a->x < b->x) { return -1; }
    if (a->x > b->x) { return 1; }
    if (a->y < b->y) { return -1; }
    if (a->y > b->y) { return 1; }

    return 0;
}

struct Prob {
    FILE * fin;
    FILE * fout;

    int n;
    Hole hs[12];
    int order[12];
    
    void readIn() {
        fin = fopen("wormhole.in", "r"); assert(fin);
        int ret = fscanf(fin, "%d", &n);
        assert(ret == 1);

        assert(n <= 12);
        assert(n % 2 == 0);

        for (int i = 0; i < n; i++) {
            ret = fscanf(fin, "%d %d", &hs[i].y, &hs[i].x);
            assert(ret == 2);
        }

        fclose(fin);
    }

    void pairIt(int id) {
        for (int i = 0; i < n; i++) {
            order[i] = i;
        }

        for (int m = n - 1; m > 1; m -= 2) {
            int off = id % m;
            id = id / m;
            
            if (off == 0) {
                continue;
            }

            int focus = n - m;
            int toSwap = focus + off;
            int t = order[focus];
            order[focus] = order[toSwap];
            order[toSwap] = t;
        }

        for (int i = 0; i < n; i+=2) {
            Hole * h1 = &hs[order[i]];
            Hole * h2 = &hs[order[i+1]];
            h1->pairing = h2;
            h2->pairing = h1;
        }
    }

    int maxid() {
        int ret = 1;
        for (int m = n - 1; m > 1; m -= 2) {
            ret *= m;
        }
        return ret;
    }

    bool findCycle() {
        for (int i = 0; i < n; i++) {
            hs[i].outChecked = false;
        }

        for (int i = 0; i < n; i++) {
            Hole *start = &hs[i];
            if (start->outChecked) {
                continue;
            }
            
            Hole *cur = start;
            while (1) {
                Hole * next = cur->next;
                assert(!cur->outChecked);
                cur->outChecked = true;
                if (next == NULL) {
                    break; // end of road
                }
                if (next->pairing->id == start->id) {
                    return true; // found a cycle
                }

                cur = next->pairing;
            }

            // mark stuff reversely
            cur = start->pairing;
            while (1) {
                Hole * last = cur->last;
                if (last == NULL) {
                    break; // end of road
                }
                assert(!last->outChecked);
                last->outChecked = true;
                assert(last->id != start->id);
                cur = last->pairing;
            }
        }
        
        return false;
    }

    void writeOut() {
        qsort(hs, n, sizeof(Hole), hcmp);
        for (int i = 0; i < n; i++) {
            hs[i].id = i;
        }

        for (int i = 0; i < n-1; i++) {
            Hole * th = &hs[i];
            Hole * nx = &hs[i+1];
            if (th->x == nx->x) {
                assert(th->y != nx->y);
                nx->last = th;
                th->next = nx;
            } else {
                nx->last = NULL;
                th->next = NULL;
            }
        }

        int max = maxid();
        int res = 0;

        for (int id = 0; id < max; id++) {
            pairIt(id);
            if (findCycle()) {
                res++;
            }
        }

        fout = fopen("wormhole.out", "w"); assert(fout);
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

