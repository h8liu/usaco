/*
ID: liulonn1
PROG: fence3
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <cmath>

inline double delta(double a, double b) {
    if (a>b) { return a-b; }
    return b-a;
}

inline double pdist(double x1, double y1, double x2, double y2) {
    double dx = delta(x1, x2);
    double dy = delta(y1, y2);
    return sqrt(dx*dx + dy*dy);
}

struct Fence {
    int ix1, iy1, ix2, iy2;
    double x1, y1;
    double x2, y2;
    bool horiz; // has same y
};

struct Dist {
    double x, y;
    double dist;
    double dxm, dxp;
    double dym, dyp;
};

struct Prob {
    FILE * fin;
    FILE * fout;

    int n;
    std::vector<Fence> fences;

    Dist center;

    Dist calcDist(double x, double y) {
        Dist dist;
        dist.x = x;
        dist.y = y;
        dist.dist = 0;
        dist.dxm = dist.dxp = 0;
        dist.dym = dist.dyp = 0;

        for (int i = 0; i < n; i++) {
            Fence& f = fences[i];
            double d;
            double dxm, dxp, dym, dyp;
            if (f.horiz) { // f.y1 == f.y2
                if (x < f.x1) {
                    d = pdist(x, y, f.x1, f.y1);
                    dxm = dxp = (x - f.x1) / d;
                    dym = dyp = (y - f.y1) / d;
                } else if (x > f.x2) {
                    d = pdist(x, y, f.x2, f.y2);
                    dxm = dxp = (x - f.x2) / d;
                    dym = dyp = (y - f.y2) / d;
                } else {
                    d = delta(y, f.y1);
                    dxm = dxp = 0;
                    if (y < f.y1) {
                        dym = dyp = -1;
                    } else if (y > f.y1) {
                        dym = dyp = 1;
                    } else {
                        dym = -1;
                        dyp = 1;
                    }
                }
            } else { // f.x1 == f.x2
                if (y < f.y1) {
                    d = pdist(x, y, f.x1, f.y1);
                    dxm = dxp = (x - f.x1) / d;
                    dym = dyp = (y - f.y1) / d;
                } else if (y > f.y2) {
                    d = pdist(x, y, f.x2, f.y2);
                    dxm = dxp = (x - f.x2) / d;
                    dym = dyp = (y - f.y2) / d;
                } else {
                    d = delta(x, f.x2);
                    dym = dyp = 0;
                    if (x < f.x1) {
                        dxm = dxp = -1;
                    } else if (x > f.x1) {
                        dxm = dxp = 1;
                    } else {
                        dxm = -1;
                        dxp = 1;
                    }
                }
            }

            dist.dist += d;
            dist.dxm += dxm;
            dist.dxp += dxp;
            dist.dym += dym;
            dist.dyp += dyp;
        }

        return dist;
    }
    
    void readIn() {
        fin = fopen("fence3.in", "r"); assert(fin);
        int ret = fscanf(fin, "%d", &n); assert(ret == 1);

        for (int i = 0; i < n; i++) {
            Fence f;
            int x1, y1, x2, y2;
            ret = fscanf(fin, "%d %d %d %d", &x1, &y1, &x2, &y2);
            assert(ret == 4);

            if (x1 > x2) {
                int t = x1;
                x1 = x2;
                x2 = t;
            }
            if (y1 > y2) {
                int t = y1;
                y1 = y2;
                y2 = t;
            }

            f.x1 = x1;
            f.y1 = y1;
            f.x2 = x2;
            f.y2 = y2;

            f.ix1 = x1;
            f.iy1 = y1;
            f.ix2 = x2;
            f.iy2 = y2;

            f.horiz = (f.iy1 == f.iy2);

            fences.push_back(f);
        }

        fclose(fin);
    }

    void solve() {
        Fence& f0 = fences[0];
        int minY = f0.iy1;
        int maxY = f0.iy2;
        int minX = f0.ix1;
        int maxX = f0.ix2;

        for (int i = 1; i < n; i++) {
            Fence& f = fences[i];
            if (f.iy1 < minY) { minY = f.iy1; }
            if (f.iy2 > maxY) { maxY = f.iy2; }
            if (f.ix1 < minX) { minX = f.ix1; }
            if (f.ix2 > maxX) { maxX = f.ix2; }
        }

        double minDist = -1;
        for (int x = minX; x <= maxX; x++) {
            for (int y = minY; y <= maxY; y++) {
                Dist dist = calcDist(x, y);
                if (minDist < 0 || dist.dist < minDist) {
                    minDist = dist.dist;
                    center = dist;
                }
            }
        }

        double stride = 0.1;
        while (stride > 0.001) {
            // printf("stride = %f\n", stride);
            // printf("center = (%f, %f)\n", center.x, center.y);
            double dx = 0;
            double dy = 0;
            if (center.dxm > 0) {
                dx = center.dxm;
            } else if (center.dxp < 0) {
                dx = center.dxp;
            }

            if (center.dym > 0) {
                dy = center.dym;
            } else if (center.dyp < 0) {
                dy = center.dyp;
            }
            // printf("d = (%f, %f)\n", dx, dy);
            // printf("dist = %f\n", center.dist);

            double dd = sqrt(dx*dx + dy*dy);
            dx = dx / dd * stride;
            dy = dy / dd * stride;

            double x = center.x - dx;
            double y = center.y - dy;

            Dist next = calcDist(x, y);
            
            // printf("next = (%f, %f)\n", x, y);
            // printf("next dist = %f\n", next.dist);

            if (next.dist < center.dist - 0.0000001) {
                center = next;
            } else {
                stride /= 2;
            }
        }
    }

    void writeOut() {
        fout = fopen("fence3.out", "w"); assert(fout);
        fprintf(fout, "%.1f %.1f %.1f\n", center.x, center.y, center.dist);
        fclose(fout);
    }
};

Prob prob;

int main() {
    prob.readIn();
    prob.solve();
    prob.writeOut();
    return 0;
}

