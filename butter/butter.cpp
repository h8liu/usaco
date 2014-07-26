/*
ID: liulonn1
PROG: butter
LANG: C++
*/

#define _P "butter"

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <queue>

using std::priority_queue;

struct D {
    int d;
    int p;

    bool operator > (const D & other) const {
        if (d == other.d) {
            return p > other.p;
        }
        return d < other.d;
    }

    bool operator < (const D & other) const {
        return other > *this;
    }
};

struct E {
    int to;
    int d;
};

struct Es {
    int n;
    E * e;
};

int main() {
    FILE * fin = fopen(_P ".in", "r");
    assert(fin);
    FILE * fout = fopen(_P ".out", "w");
    // FILE * fout = stdout;
    assert(fout);

    int n, p, c;
    int ret = fscanf(fin, "%d %d %d", &n, &p, &c);
    assert(ret == 3);

    int * ncows = new int[p];
    bzero(ncows, sizeof(int) * p);
    for (int i = 0; i < n; i++) {
        int cowp;
        ret = fscanf(fin, "%d", &cowp);
        assert(ret == 1);
        cowp -= 1;
        assert(cowp >= 0 && cowp < p);
        ncows[cowp]++;
    }

    Es * edges = new Es[p];
    for (int i = 0; i < p; i++) {
        edges[i].e = new E[p];
        edges[i].n = 0;
    }

    for (int i = 0; i < c; i++) {
        int f, t;
        int d;
        ret = fscanf(fin, "%d %d %d", &f, &t, &d);
        assert(ret == 3);
        f--; t--;
        assert(f >= 0 && f < p);
        assert(t >= 0 && t < p);
        int n;

        n = edges[f].n;
        edges[f].e[n].to = t;
        edges[f].e[n].d = d;
        edges[f].n++;

        n = edges[t].n;
        edges[t].e[n].to = f;
        edges[t].e[n].d = d;
        edges[t].n++;
    }
    fclose(fin);
    
    int minsum = -1;

    bool * visited = new bool[p];

    for (int i = 0; i < p; i++) {
        bzero(visited, sizeof(bool) * p);   
        priority_queue<D> heap;
        D d; d.p = i; d.d = 0;
        heap.push(d);
        int sum = 0;
        
        while (!heap.empty()) {
            const D & top = heap.top();
            int tp = top.p;
            int td = top.d;
            heap.pop();

            if (!visited[tp]) { // not visited, will expand this
                visited[tp] = true;
                sum += td * ncows[tp];
                int n = edges[tp].n;
                E * e = edges[tp].e;
                for (int j = 0; j < n; j++) {
                    int newp = e[j].to;
                    if (!visited[newp]) {
                        D newd;
                        newd.p = newp;
                        newd.d = e[j].d + td;
                        heap.push(newd);
                    }
                }
            }
        }
        
        if (minsum < 0 || sum < minsum) {
            minsum = sum;
        }
    }

    fprintf(fout, "%d\n", minsum);

    fclose(fout);
    return 0;
}

