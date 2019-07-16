/*
ID: liulonn1
PROG: wissqu
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <string>
#include <map>

using std::map;
using std::string;

uint16_t masks[16] = {
    0x0033,
    0x0077,
    0x00ee,
    0x00cc,

    0x0333,
    0x0777,
    0x0eee,
    0x0ccc,

    0x3330,
    0x7770,
    0xeee0,
    0xccc0,

    0x3300,
    0x7700,
    0xee00,
    0xcc00,
};

struct State {
    int n;
    string trace;
};

State makeState(int n, string t) {
    State s;
    s.n = n;
    s.trace = t;
    return s;
}

struct Prob {
    FILE * fin;
    FILE * fout;

    string init;

    map<string, State> last;
    map<string, State> trace;

    void readIn() {
        fin = fopen("wissqu.in", "r"); assert(fin);
        char line[8];
        char layout[20] = {0};

        for (int i = 0; i < 4; i++) {
            int ret = fscanf(fin, "%s", line);
            assert(ret == 1);

            assert(strlen(line) == 4);
            strncpy(&layout[i*4], line, 4);
        }

        fclose(fin);

        init = string(layout);
    }

    void findNext(const string& cur, map<string, State>& nexts, const State& base) {
        // printBoard(cur);

        uint16_t taken[5] = {0};
        int count[5] = {0};

        for (int i = 0; i < 16; i++) {
            char c = cur[i];
            int ind = 0;
            if (c >= 'A' && c <= 'E') {
                ind = (int)(c - 'A');
            }
            if (c >= 'a' && c <= 'e') {
                ind = (int)(c - 'a');
            }
            taken[ind] |= masks[i];
            count[ind]++;
        }

        for (int i = 0; i < 5; i++) {
            if (count[i] >= 4) { continue; } // impossible to add more
            uint16_t t = taken[i];
            for (int block = 0; block < 4; block++ ) {
                int offset = block * 4;
                uint16_t b = (t >> offset) & 0xf;
                if (b == 0xf) { continue; }
                for (int pos = offset; pos < offset+4; pos++) {
                    if ((t & (0x1 << pos)) != 0) { continue; }
                    char c = cur[pos];
                    if (!(c >= 'A' && c <= 'E')) { continue; }

                    string next = cur;
                    next[pos] = (char)('a'+i);
                    
                    char concat[4] = {'A'+i, '1'+block, '1'+pos-offset, 0};
                    string tr = base.trace + string(concat);

                    map<string, State>::iterator it = nexts.find(next);
                    if (it == nexts.end()) {
                        nexts[next] = makeState(base.n, tr);
                    } else {
                        it->second.n += base.n;
                        if (tr < it->second.trace) {
                            it->second.trace = tr;
                        }
                    }
                }
            }
        }
    }

    bool isEnding(const string& s) {
        int count[5] = {3, 3, 3, 4, 3};

        for (int i = 0; i < 16; i++) {
            char c = s[i];
            count[(int)(c-'a')]--;
        }

        for (int i = 0; i < 5; i++) {
            if (count[i] != 0) { return false; }
        }
        return true;
    }

    void solve() {
        last[init] = makeState(1, "");

        for (int i = 0; i < 16; i++) {
            trace.clear();
            for (map<string, State>::iterator it = last.begin();
                    it != last.end(); it++) {
                findNext(it->first, trace, it->second);
            }
            last.swap(trace);
        }

        total = 0;
        for (map<string, State>::iterator it = last.begin();
                it != last.end(); it++) {
            if (!isEnding(it->first)) { continue; }
            total += it->second.n;
            if (bestTrace.empty() || it->second.trace < bestTrace) {
                bestTrace = it->second.trace;
            }
        }
    }

    void writeOut() {
        fout = fopen("wissqu.out", "w"); assert(fout);

        for (int i = 0; i < 16; i++) {
            int off = i * 3;
            fprintf(fout, "%c %c %c\n",
                    bestTrace[off], bestTrace[off+1], bestTrace[off+2]);
        }

        fprintf(fout, "%d\n", total);
        fclose(fout);
    }

    int total;
    string bestTrace;
};

Prob prob;

int main() {
    prob.readIn();
    prob.solve();
    prob.writeOut();
    return 0;
}

