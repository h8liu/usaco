/*
ID: liulonn1
PROG: fc
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cmath>

#include <vector>

using std::vector;

struct Pt {
    double x, y;
};

struct Cos {
    Pt * pt;
    double cos;
};

int ptcmp(const Pt *p1, const Pt *p2) {
    if (p1->x < p2->x) { return -1; }
    if (p1->x > p2->x) { return 1; }
    if (p1->y < p2->y) { return -1; }
    if (p1->y > p2->y) { return 1; }
    return 0;
}

int ccmp(const void *a, const void *b) {
    const Cos & p1 = *((const Cos *)(a));
    const Cos & p2 = *((const Cos *)(b));
    if (p1.cos < p2.cos) { return -1; }
    if (p1.cos > p2.cos) { return 1; }
    return ptcmp(p1.pt, p2.pt);
}

struct Prob {
    FILE * fin;
    FILE * fout;

    int n;
    Pt pts[10000];
    Cos order[10001];

    double dist(Pt *p1, Pt *p2) {
        if (p1->x == p2->x && p1->y == p2->y) {
            return 0;
        }
        double dx = p1->x - p2->x;
        double dy = p1->y - p2->y;
        
        return sqrt(dx * dx + dy * dy);
    }

    void sub(Pt *p1, Pt *p2, Pt *res) {
        res->x = p1->x - p2->x;
        res->y = p1->y - p2->y;
    }

    double xmul(Pt *p1, Pt *p2) {
        return p1->x * p2->y - p2->x * p1->y;
    }

    bool isRightward(Pt *from, Pt *via, Pt *to) {
        Pt d1, d2;
        sub(via, from, &d1);
        sub(to, via, &d2);
        double x = xmul(&d1, &d2);
        return x < 0;
    }
    
    void readIn() {
        fin = fopen("fc.in", "r"); assert(fin);
        int ret = fscanf(fin, "%d", &n); assert(ret == 1);

        for (int i = 0; i < n; i++) {
            Pt & pt = pts[i];
            ret = fscanf(fin, "%lf %lf", &pt.x, &pt.y);
            assert(ret == 2);
        }

        fclose(fin);
    }

    Pt base;

    void writeOut() {
        fout = fopen("fc.out", "w"); assert(fout);

        // pick bottom most
        int bottomMost = 0;
        for (int i = 1; i < n; i++) {
            if (pts[i].y < pts[bottomMost].y) {
                bottomMost = i;
            }
        }

        // shift as relative to bottom most
        base = pts[bottomMost];
        for (int i = 0; i < n; i++) {
            pts[i].x -= base.x;
            pts[i].y -= base.y;
        }

        // calculate the cos
        for (int i = 0; i < n; i++) {
            Pt & pt = pts[i];
            order[i].pt = &pt;
            if (pt.x == 0 && pt.y == 0) {
                order[i].cos = -1.1;
            } else if (pt.y == 0 && pt.x < 0) {
                order[i].cos = -1;
            } else if (pt.y == 0 && pt.x > 0) {
                order[i].cos = 1;
            } else {
                order[i].cos = pt.x / sqrt(pt.x * pt.x + pt.y * pt.y);
            }
        }

        // sort
        qsort(order, n, sizeof(struct Cos), ccmp);

        order[n] = order[0]; // close the path;

        double len = hull();
        fprintf(fout, "%.2lf\n", len);

        fclose(fout);
    }

    double hull() {
        if (n <= 1) {
            return 0;
        }
        
        if (n == 2) return dist(&pts[0], &pts[1]) * 2;
        
        vector<Pt *> hull;
        
        for (int i = 0; i <= n; i++) {
            Cos & c = order[i];
            Pt *pt = c.pt;

            if (!hull.empty()) {
                Pt *last = hull.back();
                if (pt->x == last->x && pt->y == last->y) {
                    // same as last one, just skip;
                    continue;
                }
            }
         
            while (hull.size() >= 2) {
                vector<Pt *>::reverse_iterator it = hull.rbegin();
                Pt *last = *it;
                it++;
                Pt *last2 = *it;

                if (isRightward(last2, last, pt)) {
                    break;
                }

                /*
                printf("pop out (%lf, %lf) <%lf, %lf>\n", 
                        last->x + base.x, last->y + base.y,
                        last->x, last->y);
                */
                hull.pop_back();
            }
            
            /*
            printf("push in (%lf, %lf) <%lf, %lf>\n", 
                    pt->x + base.x, pt->y + base.y,
                    pt->x, pt->y);
            */
            hull.push_back(pt);
        }

        assert(!hull.empty());

        // printf("hull has %lu points\n", hull.size());

        if (hull.size() <= 2) { return 0; }
        if (hull.size() == 3) {
            Pt * p1 = hull[0];
            Pt * p2 = hull[1];
            return dist(p1, p2) * 2;
        }

        assert(hull.size() >= 3);
        unsigned int cnt = hull.size() - 1;

        double sum = 0;
        for (unsigned int i = 0; i < cnt; i++) {
            double d = dist(hull[i], hull[i+1]);
            /*
            printf("(%lf, %lf) - %lf\n", 
                    hull[i]->x + base.x,
                    hull[i]->y + base.y,
                    d);
            */
            sum += d;
        }
        return sum;
    }
};

Prob prob;

int main() {
    prob.readIn();
    prob.writeOut();
    return 0;
}

