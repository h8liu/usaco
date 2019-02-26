/*
ID: liulonn1
PROG: cowxor
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <stdint.h>
#include <algorithm>

using std::vector;

struct Max {
    int p1;
    int p2;
    uint64_t x;
    uint64_t r1;
    uint64_t r2;

    void fixRankOrder() {
        if (r1 > r2) {
            uint64_t t = r1;
            r1 = r2;
            r2 = t;
        }
    }
};

struct Prob {
    FILE * fin;
    FILE * fout;

    int n;
    vector<uint64_t> cowXorSums;

    uint32_t maxXor;
    int begin;
    int end;

    bool enableDebug;
    
    void readIn() {
        fin = fopen("cowxor.in", "r"); assert(fin);
        int ret = fscanf(fin, "%d", &n); assert(ret == 1);

        uint64_t sum = 0;
        cowXorSums.push_back(0);

        for (int i = 0; i < n; i++) {
            uint32_t t;
            ret = fscanf(fin, "%u", &t); assert(ret == 1);
            uint64_t v = static_cast<uint64_t>(t);
            sum ^= (v << 32);
            cowXorSums.push_back(sum | static_cast<uint64_t>(i+1));
        }

        fclose(fin);
    }

    uint64_t xorAt(int p) { return cowXorSums[p] >> 32; }

    uint64_t rankAt(int p) { return cowXorSums[p] & 0xffffffff; }

    int findFlip(int from, int to, uint64_t checkBit) {
        for (int i = from; i < to; i++) {
            if ((xorAt(i) & checkBit) != 0) {
                return i;
            }
        }
        return to;
    }

    Max findMax(int from, int to, uint64_t checkBit) {
        if (checkBit == 0) {
            Max ret;
            ret.p1 = from;
            ret.p2 = from+1;
            ret.x = 0;
            ret.r1 = rankAt(ret.p1);
            ret.r2 = rankAt(ret.p2);
            return ret;
        }

        int flip = findFlip(from, to, checkBit);
        if (flip == from) {
            return findMax(from, to, checkBit >> 1);
        }
        if (flip == to) {
            return findMax(from, to, checkBit >> 1);
        }
        
        return findPair(from, flip, flip, to, checkBit >> 1);
    }

    Max findPair(int from1, int to1, int from2, int to2,
            uint64_t checkBit) {
        if (enableDebug) {
            printf("%d,%d  %d,%d  %08llx\n", from1, to1, from2, to2, checkBit);
        }
        Max ret = _findPair(from1, to1, from2, to2, checkBit);

        if (enableDebug) {
            printf("%d,%d  %d,%d  %08llx -> %08llx = %08llx@%d/%d ^ %08llx@%d/%d \n",
                    from1, to1, from2, to2, checkBit,
                    ret.x,
                    xorAt(ret.p1), ret.p1, int(ret.r1),
                    xorAt(ret.p2), ret.p2, int(ret.r2));
        }
        return ret;
    }

    Max _findPair(int from1, int to1, int from2, int to2,
            uint64_t checkBit) {

        if (checkBit == 0) {
            Max ret;
            ret.p1 = from1;
            ret.p2 = from2;
            ret.x = xorAt(ret.p1) ^ xorAt(ret.p2);
            ret.r1 = rankAt(ret.p1);
            ret.r2 = rankAt(ret.p2);

            if (ret.r1 < ret.r2) {
                // we need to try to increase r1
                for (int i = from1+1; i < to1; i++) {
                    uint64_t r1 = rankAt(i);
                    if (r1 > ret.r2) { break; }
                    ret.r1 = r1;
                }
            } else {
                // need to try to increase r2
                for (int i = from2+1; i < to2; i++) {
                    uint64_t r2 = rankAt(i);
                    if (r2 > ret.r1) { break; }
                    ret.r2 = r2;
                }
            }

            ret.fixRankOrder();
            return ret;
        }

        if (to1 - from1 <= 1) {
            Max ret;
            ret.p1 = from1;
            ret.p2 = from2;
            ret.r1 = rankAt(ret.p1); // this one is fixed
            ret.r2 = rankAt(ret.p2);

            uint64_t x1 = xorAt(from1);
            ret.x = xorAt(from2) ^ x1;

            for (int i = from2+1; i < to2; i++) {
                uint64_t x = x1 ^ xorAt(i);
                uint64_t rank = rankAt(i);
                if (x > ret.x) {
                    ret.p2 = i;
                    ret.x = x;
                    ret.r2 = rank;
                } else if (x == ret.x) {
                    if (ret.r2 < ret.r1) {
                        if (rank < ret.r1 && rank > ret.r2) {
                            ret.p2 = i;
                            ret.r2 = rank;
                        }
                    } else if (rank < ret.r2) {
                        ret.p2 = i;
                        ret.r2 = rank;
                    }
                }
            }

            ret.fixRankOrder();
            return ret;
        }

        if (to2 - from2 <= 1) {
            Max ret;
            ret.p1 = from1;
            ret.p2 = from2;
            ret.r1 = rankAt(ret.p1);
            ret.r2 = rankAt(ret.p2); // this one is fixed

            uint64_t x2 = xorAt(from2);
            ret.x = xorAt(from1) ^ x2;
            for (int i = from1+1; i < to1; i++) {
                uint64_t x = x2 ^ xorAt(i);
                uint64_t rank = rankAt(i);
                if (x > ret.x) {
                    ret.p1 = i;
                    ret.x = x;
                    ret.r1 = rank;
                } else if (x == ret.x) {
                    if (ret.r1 < ret.r2) {
                        if (rank < ret.r2 && rank > ret.r1) {
                            ret.p1 = i;
                            ret.r1 = rank;
                        }
                    } else if (rank < ret.r1) {
                        ret.p1 = i;
                        ret.r1 = rank;
                    }
                }
            }

            ret.fixRankOrder();
            return ret;
        }

        uint64_t nextCheckBit = checkBit >> 1;

        int flip1 = findFlip(from1, to1, checkBit);
        int flip2 = findFlip(from2, to2, checkBit);

        bool degen1 = (flip1 == from1 || flip1 == to1);
        bool degen2 = (flip2 == from2 || flip2 == to2);

        if (degen1 && degen2) {
            return findPair(from1, to1, from2, to2, nextCheckBit);
        }

        if (degen1) {
            if (flip1 == from1) { // all ones
                return findPair(from1, to1, from2, flip2, nextCheckBit);
            }
            // all zeros
            return findPair(from1, to1, flip2, to2, nextCheckBit);
        }
        
        if (degen2) {
            if (flip2 == from2) { // all ones
                return findPair(from1, flip1, from2, to2, nextCheckBit);
            }
            // all zeros
            return findPair(flip1, to1, from2, to2, nextCheckBit);
        }

        Max max1 = findPair(from1, flip1, flip2, to2, nextCheckBit);
        Max max2 = findPair(flip1, to1, from2, flip2, nextCheckBit);

        if (max1.x > max2.x) { return max1; }
        if (max1.x < max2.x) { return max2; }
        if (max1.r2 < max2.r2) { return max1; }
        if (max1.r2 > max2.r2) { return max2; }
        if (max1.r1 > max2.r1) { return max1; }
        return max2;
    }

    void solve() {
        enableDebug = false;
        std::sort(cowXorSums.begin(), cowXorSums.end());

        int n = static_cast<int>(cowXorSums.size());
        if (enableDebug) {
            for (int i = 0; i < n; i++) {
                printf("%d %08llx %llu\n", i, xorAt(i), rankAt(i));
            }
        }
        
        Max m = findMax(0, n, 0x1 << 21);
        maxXor = static_cast<uint32_t>(m.x);
        begin = static_cast<int>(m.r1) + 1;
        end = static_cast<int>(m.r2);
    }

    void writeOut() {
        fout = fopen("cowxor.out", "w"); assert(fout);
        fprintf(fout, "%u %d %d\n", maxXor, begin, end);
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

