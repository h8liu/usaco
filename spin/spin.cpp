/*
ID: liulonn1
PROG: spin
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cmath>
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

int main() {
    FILE * fin = fopen("spin.in", "r");
    FILE * fout = fopen("spin.out", "w");
    Wheel wheels[5];
    int ret;

    for (int i = 0; i < 5; i++) {
        ret = fscanf(fin, "%d %d", &wheels[i].speed,
                &wheels[i].nwedges);
        assert(ret == 2);

        for (int j = 0; j < wheels[i].nwedges; j++) {
            ret = fscanf(fin, "%d %d", &wheels[i].wedges[j].start,
                    &wheels[i].wedges[j].extent);
            assert(ret == 2);
        }
    }
    
    /*
    int rspeed = wheels[0].speed;
    for (int i = 0; i < 5; i++) {
        wheels[i].speed -= rspeed;
    }

    assert(wheels[0].speed == 0);
    */

    int * inside = new int[360];

    int i;
    for (i = 0; i < 365; i++) {
        for (int j = 0; j < 360; j++) {
            inside[j] = 0;
        }

        for (int j = 0; j < 5; j++) {
            Wheel & w = wheels[j];
            for (int k = 0; k < w.nwedges; k++) {
                int s = w.wedges[k].start % 360; 
                int e = (s + w.wedges[k].extent) % 360;
                int r = s;
                while (true) {
                    inside[r]++;
                    if (r == e) {
                        break;
                    }
                    r = (r + 1) % 360;
                }
            }
        }
        bool found = false;
        for (int j = 0; j < 360; j++) {
            if (inside[j] >= 5) {
                found = true;
                break;
            }
        }
        if (found) {
            break;
        }

        for (int j = 0; j < 5; j++) {
            Wheel & w = wheels[j];
            for (int k = 0; k < w.nwedges; k++) {
                int & start = w.wedges[k].start;
                start = (start + w.speed) % 360;
            }
        }
    }

    if (i == 365) {
        fprintf(fout, "none\n");
    } else {
        fprintf(fout, "%d\n", i);
    }

    fclose(fin);
    fclose(fout);

    return 0;
}

