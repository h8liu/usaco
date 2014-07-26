/*
ID: liulonn1
PROG: cowtour
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cmath>

struct P {
    int x, y;
};

P * pts;
bool * conn;

inline void getspath(int n, P * pos, bool * conn, double * path) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int t = i * n + j;
            if (i != j) {
                if (conn[i * n + j]) {
                    double dx = pos[i].x - pos[j].x;
                    double dy = pos[i].y - pos[j].y;
                    path[t] = sqrt(dx * dx + dy * dy);
                } else {
                    path[t] = -1;
                }
            } else {
                path[t] = 0;
            }
            path[j * n + i] = path[t];
        }
    }

    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < i; j++) {
                int t1 = i * n + k;
                int t2 = j * n + k;
                double p1 = path[t1];
                double p2 = path[t2];
                if (p1 > 0 && p2 > 0) {
                    double s = p1 + p2;
                    int t = i * n + j;
                    if (path[t] < 0 || s < path[t]) { // update
                        path[t] = s;
                        path[j * n + i] = s;
                    } 
                }
            }
        }
    }
}

inline double maxin(double * pd, int n) {
    double ret = 0;
    for (int i = 0; i < n; i++) {
        if (pd[i] > ret) {
            ret = pd[i];
        }
    }
    return ret;
}

int main() {
    FILE * fin = fopen("cowtour.in", "r");
    int n;
    int ret = fscanf(fin, "%d", &n);
    assert(ret == 1);

    pts = new P[n];
    for (int i = 0; i < n; i++) {
        ret = fscanf(fin, "%d %d", &(pts[i].x), &(pts[i].y));
        assert(ret == 2);
    }
    
    conn = new bool[n * n];
    char * buf = new char[n + 1];

    for (int i = 0; i < n; i++) {
        ret = fscanf(fin, "%s", buf);
        assert(ret == 1);
        for (int j = 0; j < n; j++) {
            char c = buf[j];
            if (c == '0') {
                conn[i * n + j] = false;
            } else {
                assert(c == '1');
                conn[i * n + j] = true;
            }
        }
    }
    fclose(fin);

    // alg goes here
    
    double * spath = new double[n * n];
    getspath(n, pts, conn, spath); // shortest path
    
    double dret = -1;
    double dnon = maxin(spath, n * n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (spath[i * n + j] < 0) {
                double dx = pts[i].x - pts[j].x;
                double dy = pts[i].y - pts[j].y;
                double d = sqrt(dx * dx + dy * dy);
                double di = maxin(&spath[i * n], n);
                double dj = maxin(&spath[j * n], n);
                double dia = d + di + dj;
                if (dret < 0 || dia < dret) {
                    dret = dia;
                }
            }
        }
    }

    if (dnon > dret) {
        dret = dnon;
    }
    
    FILE * fout = fopen("cowtour.out", "w");
    // FILE * fout = stdout;

    fprintf(fout, "%.6f\n", dret);
    fclose(fout);
    return 0;
}
