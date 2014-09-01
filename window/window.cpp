/*
ID: liulonn1
PROG: window
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <stdint.h>
#include <map>

using std::map;

struct Wind {
    char name;
    int x1, y1;
    int x2, y2;
    Wind *above;
    Wind *below;
};

int icmp(const void *a, const void *b) {
    int x = *((int *)(a));
    int y = *((int *)(b));
    if (x < y) { return -1; }
    if (x > y) { return 1; }
    return 0;
}

struct Prob {
    FILE * fin;
    FILE * fout;

    map<char, Wind *> wm;
    Wind * top;
    Wind * bot;
    char line[4096];

    Wind * remove(char code) {
        Wind * w = wm[code];
        Wind * above = w->above;
        Wind * below = w->below;
        if (above) { 
            above->below = below; 
        } else {
            assert(w == top);
            top = below;
        }

        if (below) { 
            below->above = above; 
        } else {
            assert(w == bot);
            bot = above;
        }

        return w;
    }
    
    void putTop(Wind * w) {
        w->above = NULL;
        w->below = top;
        if (top) {
            top->above = w;
        }
        if (!bot) {
            bot = w;
        }
        top = w;
    }

    void putBot(Wind * w) {
        w->below = NULL;
        w->above = bot;
        if (bot) {
            bot->below = w;
        }
        if (!top) {
            top = w;
        }
        bot = w;
    }

    void show(Wind * w) {
        Wind * pt = top;

        int nwin = 0;
        Wind * ws[100];
        int nx = 0;
        int ny = 0;

        int xs[200];
        int ys[200];
        int xmin, xmax;
        int ymin, ymax;

        while (pt != w) {
            ws[nwin++] = pt;
            pt = pt->below;
            assert(pt != NULL);
        }

        xmin = w->x1;
        xmax = w->x2;
        ymin = w->y1;
        ymax = w->y2;
        xs[nx++] = xmin;
        xs[nx++] = xmax;
        ys[ny++] = ymin;
        ys[ny++] = ymax;

        uint64_t area = uint64_t(ymax - ymin) * uint64_t(xmax - xmin);
        
        for (int i = 0; i < nwin; i++) {
            int x = ws[i]->x1;
            if (x > xmin && x < xmax) {
                xs[nx++] = x;
            }
            x = ws[i]->x2;
            if (x > xmin && x < xmax) {
                xs[nx++] = x;
            }

            int y = ws[i]->y1;
            if (y > ymin && y < ymax) {
                ys[ny++] = y;
            }
            y = ws[i]->y2;
            if (y > ymin && y < ymax) {
                ys[ny++] = y;
            }
        }
        
        qsort(xs, nx, sizeof(int), icmp);
        qsort(ys, ny, sizeof(int), icmp);

        assert(xs[0] == xmin);
        assert(xs[nx-1] == xmax);
        assert(ys[0] == ymin);
        assert(ys[ny-1] == ymax);

        uint64_t avis = 0;
        for (int i = 0; i < nx-1; i++) {
            int x1 = xs[i];
            int x2 = xs[i+1];
            for (int j = 0; j < ny-1; j++) {
                int y1 = ys[j];
                int y2 = ys[j+1];
                bool visible = true;

                for (int t = 0; t < nwin; t++) {
                    Wind * cur = ws[t];
                    if (cur->x1 <= x1 && cur->x2 >= x2 &&
                            cur->y1 <= y1 && cur->y2 >= y2) {
                        visible = false;
                        break;
                    }
                }

                if (visible) {
                    avis += uint64_t(x2 - x1) * uint64_t(y2 - y1);
                }
            }
        }

        fprintf(fout, "%.3lf\n", double(avis) / double(area) * 100);
    }
    
    void work() {
        fin = fopen("window.in", "r"); assert(fin);
        fout = fopen("window.out", "w"); assert(fout);
        
        int ret;
        top = NULL;
        bot = NULL;

        while (true) {
            ret = fscanf(fin, "%s", line);
            if (ret == EOF) { break; }
            if (line[0] == '\0') { continue; }

            // printf("> %s\n", line);

            char cmd = line[0];
            assert(line[1] == '(');
            char code = line[2];
            assert(line[3] == ',' || line[3] == ')');

            if (cmd == 't') {
                Wind * w = remove(code);
                putTop(w);
            } else if (cmd == 'b') {
                Wind * w = remove(code);
                putBot(w);
            } else if (cmd == 'd') {
                Wind * w = remove(code);
                wm.erase(code);
                delete w;
            } else if (cmd == 'w') {
                assert(line[3] == ',');
                int i = 0;
                char * opts[4];
                char * pt = &line[3];

                while (*pt != '\0') {
                    if (*pt == ',') {
                        *pt = '\0';
                        opts[i] = &pt[1];
                        i++;
                    } else if (*pt == ')') {
                        *pt = '\0';
                    }
                    pt++;
                }

                assert(i == 4);
                int x1 = atoi(opts[0]);
                int y1 = atoi(opts[1]);
                int x2 = atoi(opts[2]);
                int y2 = atoi(opts[3]);
                assert(x1 != x2);
                assert(y1 != y2);

                Wind * w = new Wind();
                if (x1 < x2) {
                    w->x1 = x1;
                    w->x2 = x2;
                } else {
                    w->x2 = x1;
                    w->x1 = x2;
                }

                if (y1 < y2) {
                    w->y1 = y1;
                    w->y2 = y2;
                } else {
                    w->y2 = y1;
                    w->y1 = y2;
                }
                w->name = code;
                
                assert(wm.find(code) == wm.end());
                wm[code] = w;
                putTop(w);
            } else if (cmd == 's') {
                show(wm[code]);
            }
        }

        fclose(fin);
        fclose(fout);
    }
};

Prob prob;

int main() {
    prob.work();
    return 0;
}

