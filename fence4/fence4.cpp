/*
ID: liulonn1
PROG: fence4
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cmath>

struct Pos { 
    int x, y; 
};

struct RPos {
    double x, y, th;
};

struct DPos {
    double x, y;
};

double PI = 3.14159265359l;
double eps = 1e-10;

struct Prob {
    char path[50];
    char name[50];
    void setName(const char * s) { strcpy(name, s); }

    int n;
    Pos ob;
    Pos corners[200];
    RPos rs[200];
    DPos mids[200];
    double ths2[200];
    int outest[200];
    double outDist[200];
    bool visible[200];

    void readIn() {
        sprintf(path, "%s.in", name);
        FILE * fin = fopen(path, "r"); 
        assert(fin);
        int ret;
        
        ret = fscanf(fin, "%d", &n);
        assert(ret == 1);
        ret = fscanf(fin, "%d %d", &ob.x, &ob.y);
        assert(ret == 2);
        for (int i = 0; i < n; i++) {
            ret = fscanf(fin, "%d %d", &corners[i].x, &corners[i].y);
            assert(ret == 2);
        }

        fclose(fin);
    }

    void printInput() {
        printf("%d\n", n);
        printf("%d, %d\n", ob.x, ob.y);
        for (int i = 0; i < n; i++) {
            printf("%d, %d\n", corners[i].x, corners[i].y);
        }
    }
    
    
    static double absd(double d) {
        if (d >= 0) return d;
        return -d;
    }

    static bool isZero(double d) {
        if (absd(d) < eps) return true;
        return false;
    }

    bool isCrossing2(Pos & base, DPos & mid, Pos & p1, Pos & p2, 
            double & dist) {
        double x1 = p1.x - base.x;
        double y1 = p1.y - base.y;
        double x2 = p2.x - base.x;
        double y2 = p2.y - base.y;
        
        double x = mid.x;
        double y = mid.y;
        
        double d = x1 * y2 - y1 * x2;
        if (isZero(d)) return false;
        
        double a = (x * y2 - x2 * y) / d;
        double b = (- x * y1 + x1 * y) / d;
        if (a < 0) return false;
        if (b < 0) return false;
        double sum = a + b;
        double tx = (a * x1 + b * x2) / sum;
        double ty = (a * y1 + b * y2) / sum;
        dist = tx * tx + ty * ty;
        return true;
    }


    bool isIntersect(Pos & a1, Pos & a2, Pos & b1, Pos & b2) {
        double mx = b1.x - a1.x;
        double my = b1.y - a1.y;
        double nx = b2.x - a1.x;
        double ny = b2.y - a1.y;
        double tx = a2.x - a1.x;
        double ty = a2.y - a1.y;

        /*
        printf("a1 = (%d, %d)\n", a1.x, a1.y);
        printf("a2 = (%d, %d)\n", a2.x, a2.y);
        printf("b1 = (%d, %d)\n", b1.x, b1.y);
        printf("b2 = (%d, %d)\n", b2.x, b2.y);

        printf("m = (%f, %f)\n", mx, my);
        printf("n = (%f, %f)\n", nx, ny);
        printf("t = (%f, %f)\n", tx, ty);
        */

        /*
        | mx  nx | "a  = "tx
        | my  ny |  b"    ty"
        
        "a  =  1/d | ny -nx | "tx
         b"        | -my mx |  ty"

         d = mx * ny - my * nx
        */
        double d = mx * ny - my * nx;
        // printf("d = %f\n", d);
        if (!isZero(d)) {
            double a = (tx * ny - nx * ty) / d;
            double b = (- tx * my + mx * ty) / d;
            if (a > -eps && b > -eps && a + b > 1 - eps) {
                return true;
            }
            return false;
        } else {
            double a = (tx * ny - nx * ty);
            if (!isZero(a)) return false;

            if (!isZero(mx)) {
                double a = my - ty;
                double b = ny - ty;
                if (a < eps && b < eps) return false;
                if (a > -eps && b > -eps) return false;
                return true;
            } else {
                double a = mx - tx;
                double b = nx - tx;
                if (a < eps && b < eps) return false;
                if (a > -eps && b > -eps) return false;
                return true;
            }
        }
    }

    bool isCrossing(Pos & a1, Pos & a2, Pos & a3) {
        double x1 = a3.x - a1.x;
        double y1 = a3.y - a1.y;
        double x2 = a2.x - a1.x;
        double y2 = a2.y - a1.y;

        double d = x1 * y2 - x2 * y1;
        if (!isZero(d)) return false;

        double r = x1 * x2 + y1 * y2;
        if (r > -eps && r < x2 * x2 + y2 * y2 + eps) {
            return true;
        }
        return false;
    }

    Pos & getCorner(int i) {
        return corners[i % n];
    }

    RPos & getR(int i) {
        return rs[i % n];
    }


    bool isValid() {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n - 3; j++) {
                if (isIntersect(getCorner(i),
                            getCorner(i + 1),
                            getCorner(i + 2 + j),
                            getCorner(i + 3 + j))) {
                    printf("intersect %d %d %d %d\n",
                            i, i + 1, i + 2 + j,
                            i + 3 + j);
                    return false;
                }
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n - 2; j++) {
                if (isCrossing(getCorner(i),
                            getCorner(i + 1),
                            getCorner(i + 2 + j))) {
                    printf("iscrossing %d %d %d\n",
                            i, i + 1, i + 2 + j);
                    return false;
                }
            }
        }
        
        return true;
    }

    void writeOut() {
        sprintf(path, "%s.out", name);
        FILE * fout = fopen(path, "w"); 
        assert(fout);

        if (!isValid()) {
            fprintf(fout, "NOFENCE\n");
        } else {
            computeVisible(fout);
        }

        fclose(fout);
    }

    double relTh(double base, double th) {
        double ret = th - base;
        if (ret >= 180) {
            ret -= 360;
        }
        return ret;
    }

    double getTh(double x, double y) {
        if (isZero(x)) {
            if (y > -eps) return 90;
            return 270;
        }
        if (isZero(y)) {
            if (x > -eps) return 0;
            return 180;
        } 

        double ret = atan(y / x) * (180 / PI);
        if (x > 0 && y < 0) return ret + 360;
        if (x < 0) return ret + 180;
        return ret;
    }
    
    void computeVisible(FILE * fout) {
        for (int i = 0; i < n; i++) {
            double x = corners[i].x - ob.x;
            double y = corners[i].y - ob.y;
            rs[i].x = x;
            rs[i].y = y;
            rs[i].th = getTh(x, y);
        }

        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (rs[i].th > rs[j].th) {
                    RPos t = rs[i];
                    rs[i] = rs[j];
                    rs[j] = t;
                }
            }
        }

        int nmid = 0;
        for (int i = 0; i < n; i++) {
            RPos & r1 = getR(i);
            RPos & r2 = getR(i + 1);
            if (!isZero(r1.th - r2.th)) {
                mids[nmid].x = r1.x + r2.x;
                mids[nmid].y = r1.y + r2.y;
                nmid++;
            }
        }

        for (int i = 0; i < n; i++) {
            visible[i] = false;
        }

        for (int i = 0; i < nmid; i++) {
            outest[i] = -1;
            outDist[i] = -1;
            DPos & t = mids[i];
            // printf("%f %f\n", t.x, t.y);

            for (int j = 0; j < n; j++) {
                Pos & p1 = getCorner(j);
                Pos & p2 = getCorner(j + 1);
                double dist = 0;
                if (isCrossing2(ob, t, p1, p2, dist)) {
                    if (outest[i] < 0 || dist < outDist[i]) {
                        outest[i] = j;
                        outDist[i] = dist;
                    }
                }
            }
        }

        for (int i = 0; i < nmid; i++) {
            visible[outest[i]] = true;
        }

        int nvisible = 0;
        for (int i = 0; i < n; i++) {
            if (visible[i]) {
                nvisible++;
            }
        }

        fprintf(fout, "%d\n", nvisible);
        for (int i = 0; i < n - 2; i++) printPair(fout, i);
        printPairReverse(fout, n - 1);
        printPair(fout, n - 2);
    }

    void printPair(FILE * fout, int i) {
        if (visible[i]) {
            Pos & p1 = getCorner(i);
            Pos & p2 = getCorner(i + 1);
            fprintf(fout, "%d %d %d %d\n", p1.x, p1.y, p2.x, p2.y);
        }
    }

    void printPairReverse(FILE * fout, int i) {
        if (visible[i]) {
            Pos & p2 = getCorner(i);
            Pos & p1 = getCorner(i + 1);
            fprintf(fout, "%d %d %d %d\n", p1.x, p1.y, p2.x, p2.y);
        }
    }
};

int main() {
    Prob prob;
    prob.setName("fence4");
    prob.readIn();
    prob.writeOut();

    return 0;
}

