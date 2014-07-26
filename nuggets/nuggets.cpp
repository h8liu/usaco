/*
ID: liulonn1
PROG: nuggets
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <queue>
#include <vector>
#include <functional>
#include <algorithm>

#define uint64_t unsigned long long

using std::priority_queue;
using std::vector;
using std::greater;


struct Prob {
    FILE * fin;
    FILE * fout;
    
    int n;
    uint64_t minbs;
    uint64_t bs[10];

    priority_queue<uint64_t, vector<uint64_t>, greater<uint64_t> > points;
    uint64_t last;
    uint64_t count;

    void readIn() {
        fin = fopen("nuggets.in", "r"); assert(fin);
        int ret;

        ret = fscanf(fin, "%d", &n); assert(ret == 1);
        minbs = ~0l;
        for (int i = 0; i < n; i++) {
            ret = fscanf(fin, "%Ld", &bs[i]);
            if (bs[i] < minbs) {
                minbs = bs[i];
            }
            assert(ret == 1);
        }

        fclose(fin);
    }

    uint64_t anydiv() {
        for (uint64_t i = 2; i <= minbs; i++) {
            bool allpassed = true;
            for (int j = 0; j < n; j++) {
                if (bs[j] % i != 0) {
                    allpassed = false;
                    break;
                }
            }
            if (allpassed) 
                return i;
        }
        return 1;
    }

    void writeOut() {
        fout = fopen("nuggets.out", "w"); assert(fout);
        
        if (anydiv() != 1) {
            fprintf(fout, "0\n");
        } else {
            for (int i = 0; i < n; i++) {
                points.push(bs[i]);
            }
            last = 0; count = 1;

            uint64_t result = 0;
            while (!points.empty()) {
                uint64_t t = points.top();
                points.pop();
                if (t == last) continue;
                if (t == last + 1) {
                    count++;
                    if (count >= minbs) { // consecutive enough
                        if (t > minbs) {
                            result = t - minbs - 1;
                        } else {
                            result = 0;
                        }
                        break;
                    }
                } else {
                    count = 0;
                }

                printf("%Ld %Ld\n", t, count);

                for (int i = 0; i < n; i++) {
                    points.push(t + bs[i]);
                }
                last = t;
            }
            fprintf(fout, "%Ld\n", result);
        }

        fclose(fout);
    }
};

Prob prob;

int main() {
    prob.readIn();
    prob.writeOut();
    return 0;
}

