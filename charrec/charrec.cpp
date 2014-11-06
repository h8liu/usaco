/*
ID: liulonn1
PROG: charrec
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <stdint.h>

struct Code {
    uint32_t lines[20];
};

struct State {
    int nout;
    uint32_t dist;
    char out;
    int last;
    bool valid;
};

struct Prob {
    FILE * fin;
    FILE * fout;

    Code font[27];
    State *res;

    uint32_t encode(char *line) {
        uint32_t c = 0;
        for (int j = 0; j < 20; j++) {
            if (line[j] == '1') {
                c |= 0x1 << j;
            }
        }
        return c;
    }
    
    void loadFont() {
        fin = fopen("font.in", "r"); assert(fin);
        int n;
        int ret = fscanf(fin, "%d", &n); 
        assert(ret == 1); assert(n == 540);
        char line[32];
        for (int cp = 0; cp < 27; cp++) {
            Code & code = font[cp];
            for (int i = 0; i < 20; i++) {
                ret = fscanf(fin, "%s", line);
                assert(ret == 1);
                code.lines[i] = encode(line);
            }
        }
        
        fclose(fin);
    }

    int nline;
    uint32_t lines[1200];

    void readIn() {
        fin = fopen("charrec.in", "r"); assert(fin);
        int ret = fscanf(fin, "%d", &nline); assert(ret == 1);
        
        char line[32];
        for (int i = 0; i < nline; i++) {
            ret = fscanf(fin, "%s", line);
            assert(ret == 1);
            lines[i] = encode(line);
        }

        fclose(fin);
    }

    int nbit(uint32_t c1, uint32_t c2) {
        uint32_t c = c1 ^ c2;
        int ret = 0;
        for (int i = 0; i < 20; i++) {
            ret += (c >> i) & 0x1;
        }
        return ret;
    }

    int dist20(int off, Code & f) {
        int ret = 0;
        for (int i = 0; i < 20; i++) {
            ret += nbit(f.lines[i], lines[off+i]);
            if (ret > 6 * 20) {
                return 200;
            }
        }
        return ret;
    }

    int dist21(int off, Code & f, int dup) {
        int ret = 0;
        for (int i = 0; i < 20; i++) {
            if (i == dup) {
                int d = nbit(f.lines[i], lines[off+i]);
                int d2 = nbit(f.lines[i], lines[off+i+1]);
                if (d < d2) { ret += d; } else { ret += d2; }
            } else if (i < dup) {
                ret += nbit(f.lines[i], lines[off+i]);
            } else {
                assert(i > dup);
                ret += nbit(f.lines[i], lines[off+i+1]);
            }

            if (ret > 6 * 20) {
                return 200;
            }
        }
        return ret;
    }

    int dist21s(int off, Code & f) {
        int ret = 200;
        for (int i = 0; i < 20; i++) {
            int d = dist21(off, f, i);
            if (d < ret) {
                ret = d;
            }
        }
        return ret;
    }

    int dist19(int off, Code & f, int miss) {
        int ret = 0;
        for (int i = 0; i < 20; i++) {
            if (i == miss) { continue; }
            if (i < miss) {
                ret += nbit(f.lines[i], lines[off+i]);
            } else {
                assert(i > miss);
                ret += nbit(f.lines[i], lines[off+i-1]);
            }

            if (ret > 6 * 19) {
                return 200;
            }
        }
        return ret;
    }

    int dist19s(int off, Code & f) {
        int ret = 200;
        for (int i = 0; i < 20; i++) {
            int d = dist19(off, f, i);
            if (d < ret) {
                ret = d;
            }
        }
        return ret;
    }
    
    State states[1201];

    void writeOut() {
        for (int i = 0; i <= nline; i++) {
            states[i].valid = false;
        }

        State & s = states[0];
        s.nout = 0;
        s.dist = 0;
        s.last = 0;
        s.valid = true;

        for (int off = 0; off <= nline - 19; off++) {
            State & s = states[off];
            if (!s.valid) {
                continue;
            }

            for (int i = 0; i < 27; i++) {
                Code & c = font[i];
                
                if (off + 20 <= nline) {
                    uint32_t d = dist20(off, c);
                    if (d < 6 * 20) {
                        trySwap(off, i, d, 20);
                    }
                }

                if (off + 21 <= nline) {
                    uint32_t d = dist21s(off, c);
                    if (d < 6 * 20) {
                        trySwap(off, i, d, 21);
                    }
                }

                if (off + 19 <= nline) {
                    uint32_t d = dist19s(off, c);
                    if (d < 6 * 19) {
                        trySwap(off, i, d, 19);
                    }
                }
            }
        }


        fout = fopen("charrec.out", "w"); assert(fout);
        if (!states[nline].valid) {
            fprintf(fout, "<no match>\n");
        } else {
            int pt = nline;
            char buf[100];

            buf[states[pt].nout] = '\0';
            while (pt > 0) {
                State & s = states[pt];
                buf[s.nout-1] = mapChar(s.out);
                pt = s.last;
            }

            fprintf(fout, "%s\n", buf);
        }

        fclose(fout);
    }

    char mapChar(char i) {
        if (i == 0) {
            return ' ';
        }
        return 'a' + i - 1;
    }

    bool trySwap(int last, int i, uint32_t d, int add) {
        int newOff = last + add;
        uint32_t newDist = states[last].dist + d;
        if (!states[newOff].valid || newDist < states[newOff].dist) {
            State & ns = states[newOff];
            ns.last = last;
            ns.nout = states[last].nout + 1;
            ns.dist = newDist;
            ns.out = char(i);
            ns.valid = true;
            return true;
        } else {
            return false;
        }
    }
};

Prob prob;

int main() {
    prob.loadFont();
    prob.readIn();
    prob.writeOut();
    return 0;
}

