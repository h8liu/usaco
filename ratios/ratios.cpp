/*
ID: liulonn1
PROG: ratios
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>

struct Wedge {
    int start;
    int extent;
};

struct Wheel {
    int speed;
    int nwedges;
    Wedge wedges[5];
};

int x1, y1, z1;
int x2, y2, z2;
int x3, y3, z3;
int xt, yt, zt;

inline int getn(int s, int t) {
    if (t > 0 && s % t == 0) {
        return s / t;
    }
    if (t == 0 && s == 0) {
        return -2; // any value
    }
    return -1; // none
}

inline bool checkeq(int n1, int n2) {
    if (n1 == -1) return false;
    if (n2 == -1) return false;
    if (n1 == -2) return true;
    if (n2 == -2) return true;
    return n1 == n2;
}

inline int maxof(int n1, int n2, int n3) {
    int m = n1;
    if (m < n2) m = n2;
    if (m < n3) m = n3;
    return m;
}

inline int checkwith(int i, int j, int k) {
    int sx = i * x1 + j * x2 + k * x3;
    int sy = i * y1 + j * y2 + k * y3;
    int sz = i * z1 + j * z2 + k * z3;
    
    int nx = getn(sx, xt);
    int ny = getn(sy, yt); 
    int nz = getn(sz, zt); 
    
    if (checkeq(nx, ny) && checkeq(ny, nz) && checkeq(nz, nx))
        return maxof(nx, ny, nz);
    
    return 0;
}

int main() {
    FILE * fin = fopen("ratios.in", "r");
    FILE * fout = fopen("ratios.out", "w");
    
    int ret;
    ret = fscanf(fin, "%d %d %d", &xt, &yt, &zt);
    assert(ret == 3);
    ret = fscanf(fin, "%d %d %d", &x1, &y1, &z1);
    assert(ret == 3);
    ret = fscanf(fin, "%d %d %d", &x2, &y2, &z2);
    assert(ret == 3);
    ret = fscanf(fin, "%d %d %d", &x3, &y3, &z3);
    assert(ret == 3);
    fclose(fin);

    int nx = 0, ny = 0, nz = 0, m = 0;

    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            for (int k = 0; k < 100; k++) {
                int ret = checkwith(i, j, k);
                if (ret > 0) {
                    if (m == 0 || ret < m) {
                        m = ret;
                        nx = i;
                        ny = j;
                        nz = k;
                    }
                }
            }
        }
    }

    if (m == 0) {
        fprintf(fout, "NONE\n");
    } else {
        fprintf(fout, "%d %d %d %d\n", nx, ny, nz, m);
    }

    fclose(fout);

    return 0;
}

