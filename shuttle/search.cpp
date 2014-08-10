/*
ID: liulonn1
PROG: shuttle
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <stdint.h>

#include <vector>
#include <set>
#include <deque>
using std::vector;
using std::set;
using std::deque;

inline uint32_t getBit(uint32_t x, uint32_t i) {
    return (x >> i) & 0x1;
}

inline uint32_t setBit(uint32_t x, uint32_t i, uint32_t b) {
    uint32_t mask = 0x1 << i;
    uint32_t apply = b << i;
    return (x & (~mask)) | (apply & mask);
}

struct State {
    uint32_t code;

    uint32_t blankPos() { return code & 0xff; }
    bool valid() { return blankPos() > 0; }
    uint32_t marbles() { return code >> 8; }

    State l2(int n) {
        State ret;
        
        uint32_t pos = blankPos();
        if (pos <= 2) { ret.code = 0; return ret; }
        uint32_t newPos = pos - 2;
        ret.code = newPos;
        
        uint32_t m = marbles();
        
        // swap the bits
        uint32_t b1 = getBit(m, newPos - 1);
        uint32_t b2 = getBit(m, newPos);
        m = setBit(m, newPos - 1, b2);
        m = setBit(m, newPos, b1);
        
        ret.code |= (m << 8);
        return ret;
    }

    State r2(int n) {
        State ret;
        uint32_t pos = blankPos();
        if (pos == 0 || pos + 2 > 2*n + 1) { ret.code = 0; return ret; }
        uint32_t newPos = pos + 2;
        ret.code = newPos;

        uint32_t m = marbles();
        uint32_t b1 = getBit(m, pos - 1);
        uint32_t b2 = getBit(m, pos);
        m = setBit(m, pos, b1);
        m = setBit(m, pos - 1, b2);

        ret.code |= (m << 8);
        return ret;
    }

    State l1(int n) {
        State ret;
        uint32_t pos = blankPos();
        if (pos <= 1) { ret.code = 0; return ret; }
        uint32_t newPos = pos - 1;
        ret.code = (code & (~0xff)) | newPos;
        return ret;
    }

    State r1(int n) {
        State ret;
        uint32_t pos = blankPos();
        if (pos == 0 || pos + 1 > 2*n + 1 ) { ret.code = 0; return ret; }
        uint32_t newPos = pos + 1;
        ret.code = (code & (~0xff)) | newPos;
        return ret;
    }

    void print(FILE *out, int n) {
        uint32_t m = marbles();
        for (int i = 0; i < n * 2; i++) {
            if (i + 1 == int(code & 0xff)) { fprintf(out, " "); }
            uint32_t b = getBit(m, i);
            if (b == 1) {
                fprintf(out, "B");
            } else {
                fprintf(out, "W");
            }
        }
        fprintf(out, "\n");
    }
};



struct Prob {
    FILE * fin;
    FILE * fout;

    int n;
    
    State startState() {
        State ret;
        ret.code = uint32_t(n + 1) | ((uint32_t(0x1 << n)) - 1) << 8;
        return ret;
    }

    State endState() {
        State ret;
        ret.code = uint32_t(n + 1) | ((uint32_t(0x1 << n)) - 1) << (8 + n);
        return ret;
    }

    void readIn() {
        fin = fopen("shuttle.in", "r"); assert(fin);
        int ret = fscanf(fin, "%d", &n); assert(ret == 1);
        fclose(fin);
    }

    vector<State> q;
    vector<int> tb; // traceback;
    set<uint32_t> visited;
    unsigned int pt;

    void extend(State next) {
        if (!next.valid()) { return; }
        if (visited.find(next.code) != visited.end()) {
            return;
        }

        q.push_back(next);
        tb.push_back(pt);
        assert(q.size() == tb.size());
        visited.insert(next.code);
    }

    void writeOut() {
        State start = startState();
        q.push_back(start);
        tb.push_back(-1);
        visited.insert(start.code);
        pt = 0;

        State end = endState();

        bool found = false;
        while (pt < q.size()) {
            State cur = q[pt];
            
            if (cur.code == end.code) {
                found = true;
                break; // reached first end state;
            }

            extend(cur.l2(n));
            extend(cur.l1(n));
            extend(cur.r1(n));
            extend(cur.r2(n));

            pt++;
        }
        
        assert(found);

        deque<int> history;

        while (pt > 0) {
            history.push_front(pt);
            pt = tb[pt];
        }

        fout = fopen("shuttle.out", "w"); assert(fout);

        int count = 0;
        for (deque<int>::iterator it = history.begin();
                it != history.end(); it++) {
            
            if (count % 20 > 0) {
                fprintf(fout, " ");
            }

            fprintf(fout, "%d", int(q[*it].blankPos()));

            count++;
            if (count % 20 == 0) {
                fprintf(fout, "\n");
            }

            // q[*it].print(fout, n);
        }

        if (count % 20 > 0) {
            fprintf(fout, "\n");
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

