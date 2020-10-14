/*
ID: liulonn1
PROG: betsy
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <map>
#include <vector>

using std::map;
using std::vector;

static const uint64_t u1 = 1;
static const int downFaceOffset = 7*4;

struct Prob {
    int n;
    int result;

    void readIn() {
        FILE * fin = fopen("betsy.in", "r"); assert(fin);
        int ret = fscanf(fin, "%d", &n);
        assert(ret == 1);
        fclose(fin);
    }

    map<uint64_t, int> cur;
    map<uint64_t, int> next;

    void setState(uint64_t* s, int pos, uint64_t v) {
        *s &= ~((uint64_t(0xf) << (pos*4)));
        *s |= (v & 0xf) << (pos*4);
    }

    void setPeer(uint64_t* s, int pos, uint64_t p) {
        *s &= ~((uint64_t(0xe) << (pos*4)));
        *s |= (p & 0x7) << (pos*4+1);
    }

    void addNext(map<uint64_t, int>& next, uint64_t s, int n) {
        map<uint64_t, int>::iterator it = next.find(s);
        if (it == next.end()) {
            next[s] = n;
        } else {
            it->second += n;
        }
    }

    void solve() {
        if (n == 1) {
            result = 1;
            return;
        }

        result = 0;

        uint64_t initState = 0;
        setState(&initState, 0, (uint64_t(n-1) << 1) | u1);
        setState(&initState, n-1, u1);
        cur[initState] = 1;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                next.clear();
                for (map<uint64_t, int>::iterator it =
                        cur.begin(); it != cur.end(); it++) {
                    uint64_t state = it->first;
                    int count = it->second;

                    int offset = j*4;
                    uint64_t rightFace = (state >> offset) & 0xf;
                    uint64_t rightArrow = rightFace & 0x1;
                    int rightPeer = int(rightFace >> 1);
                    uint64_t downFace = 0;
                    if (j > 0) {
                        downFace = (state >> downFaceOffset) & 0xf;
                    }
                    uint64_t downArrow = downFace & 0x1;
                    int downPeer = int(downFace >> 1);
                    if (downArrow == 1 && rightArrow == 1) {
                        if (rightPeer != 7) {
                            assert(downPeer != j);
                            uint64_t nextState = state;
                            setState(&nextState, j, 0);
                            setState(&nextState, 7, 0);
                            setPeer(&nextState, rightPeer, downPeer);
                            setPeer(&nextState, downPeer, rightPeer);
                            addNext(next, nextState, count);
                        }
                        if (rightPeer == 7) {
                            assert(downPeer == j);
                            if (i == n-1 && j == n-1) {
                                result += count;
                            }
                        }
                    } else if (downArrow == 1 && rightArrow == 0) {
                        if (j != n-1) {
                            uint64_t nextState = state; // no change
                            addNext(next, nextState, count);
                        }
                        if (i != n-1) {
                            uint64_t nextState = state;
                            setState(&nextState, 7, 0);
                            setState(&nextState, j, downFace);
                            setPeer(&nextState, downPeer, j);
                            addNext(next, nextState, count);
                        }
                    } else if (downArrow == 0 && rightArrow == 1) {
                        if (i != n-1) {
                            uint64_t nextState = state; // no change
                            addNext(next, nextState, count);
                        }
                        if (j != n-1) {
                            uint64_t nextState = state;
                            setState(&nextState, 7, rightFace);
                            setState(&nextState, j, 0);
                            setPeer(&nextState, rightPeer, 7);
                            addNext(next, nextState, count);
                        }
                    } else if (downArrow == 0 && rightArrow == 0) {
                        if (i != n-1 && j != n-1) {
                            uint64_t nextState = state;
                            setState(&nextState, 7, (uint64_t(j) << 1) | u1);
                            setState(&nextState, j, (uint64_t(7) << 1) | u1);
                            addNext(next, nextState, count);
                        }
                    }
                }

                cur.swap(next);
            }
        }
    }

    void writeOut() {
        FILE* fout = fopen("betsy.out", "w"); assert(fout);
        fprintf(fout, "%d\n", result);
        fclose(fout);
    }
};

static Prob prob;

int main() {
    prob.readIn();
    prob.solve();
    prob.writeOut();
    return 0;
}
