/*
ID: liulonn1
PROG: latin
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <map>
#include <algorithm>
#include <stdint.h>

using std::vector;
using std::map;

const uint64_t u1 = 1;

inline void setBit(uint64_t* v, int n, int i, int pos) {
    *v |= u1 << (n*(n-1-i) + pos);
}

struct Permu {
    char v[7];

    bool operator < (const Permu& other) const {
        return encode(7) < other.encode(7);
    }

    void clear(int n) {
        for (int i = 0; i < n; i++) {
            v[i] = 0;
        }
    }

    uint64_t encode(int n) const {
        uint64_t ret = 0;
        for (int i = 0; i < n; i++) {
            setBit(&ret, n, i, v[i]);
        }
        return ret;
    }
};

struct BigInt {
    vector<char> d;

    void set(int i) {
        d.clear();
        while (i > 0) {
            d.push_back(i % 10);
            i /= 10;
        }
    }

    void mul(char v) {
        vector<char> res;
        res.clear();
        char r = 0;
        for (vector<char>::iterator it = d.begin();
                it != d.end(); it++) {
            char x = *it * v + r;
            res.push_back(x % 10);
            r = x / 10;
        }
        if (r > 0) {
            res.push_back(r);
        }
        d.swap(res);
    }

    void print(FILE* out) {
        if (d.size() == 0) {
            fprintf(out, "0");
        }
        for (vector<char>::reverse_iterator it = d.rbegin();
                it != d.rend(); it++) {
            fprintf(out, "%d", *it);
        }
    }
};

static int bitValue(uint64_t v, int n) {
    if (v & 0x1) return 0;
    if (v & 0x2) return 1;
    if (v & 0x4) return 2;
    if (v & 0x8) return 3;
    if (v & 0x10) return 4;
    if (v & 0x20) return 5;
    if (v & 0x40) return 6;
    if (v & 0x80) return 7;
    for (int i = 0; i < n; i++) {
        if (v & (u1 << i)) {
            return i;
        }
    }
    return 9;
}

/*
static void mapInc(map<int, int>& m, int k) {
    map<int, int>::iterator it = m.find(k);
    if (it == m.end()) {
        m[k] = 1;
    } else {
        it->second++;
    }
}

static void printMap(const char* title, map<int, int>& m) {
    printf("%s\n", title);
    for (map<int, int>::iterator it = m.begin();
            it != m.end(); it++) {
        printf("  %d : %d\n", it->first, it->second);
    }
}

static void printRow(uint64_t row, int n) {
    for (int i = 0; i < n; i++) {
        int v = bitValue((row >> (n*i)) & 0xff, n);
        printf("%d", v);
    }
    printf("\n");
}

static void printFirstRows(uint64_t* matrix, int n, int f) {
    for (int i = 0; i < f; i++) {
        printRow(matrix[i], n);
    }
}
*/

struct Prob {
    FILE * fin;
    FILE * fout;
    int n;
    uint64_t mask;
    BigInt result;

    vector<uint64_t> permus;
    uint64_t permuBan;
    uint64_t matrix[7];
    uint64_t matrixt[7];

    int t1circ1, t1circ2; // 2nd row: 10...
    int t2circ1, t2circ2, t2circ3; // 2nd row: 123..
    int t3circ1, t3circ2; // 2nd row: 120..

    /*
    map<int, int> m1;
    map<int, int> m2;
    map<int, int> m3;
    map<int, int> m4;
    */

    int offset(int i) { return n*(n-1-i); }

    uint64_t getBitset(uint64_t v, int i) {
        return (v >> offset(i)) & mask;
    }

    int getBitValue(uint64_t v, int i) {
        return bitValue(getBitset(v, i), n);
    }

    void setClear(uint64_t* v, int i) {
        *v &= ~(mask << offset(i));
    }

    void setBitset(uint64_t* v, int i, uint64_t set) {
        setClear(v, i);
        *v |= (set & mask) << offset(i);
    }

    void setFillAfter(uint64_t* v, int i) {
        if (i == 0) {
            *v = (u1 << (n*n)) - 1;
            return;
        }
        *v |= (u1 << offset(i-1)) -1;
    }

    void setClearAfter(uint64_t* v, int i) {
        if (i == 0) { *v = 0; return; }
        int o = offset(i-1);
        *v >>= o;
        *v <<= o;
    }

    void setMatrix(int row, int col, int v) {
        setBitset(&matrix[row], col, u1 << v);
        setBitset(&matrixt[col], row, u1 << v);
    }
    
    void readIn() {
        fin = fopen("latin.in", "r"); assert(fin);
        int ret = fscanf(fin, "%d", &n);
        assert(ret == 1);
        fclose(fin);
    }
    
    void permuGen(Permu* base, int pos, vector<uint64_t>* output) {
        if (pos == n-1) {
            uint64_t v = base->encode(n);
            if ((v & permuBan) == 0) {
                output->push_back(v);
            }
            return;
        }

        permuGen(base, pos+1, output);
        char hold = base->v[pos];
        for (int i = pos+1; i < n; i++) {
            base->v[pos] = base->v[i];
            base->v[i] = hold;

            permuGen(base, pos+1, output);
            
            base->v[i] = base->v[pos];
        }
        base->v[pos] = hold;
    }

    void solve() {
        t1circ1 = -1; t1circ2 = -1;
        t2circ1 = -1; t2circ2 = -1; t2circ3 = -1;
        t3circ1 = -1; t3circ2 = -1;
        mask = (u1 << n) - 1;

        Permu p;
        for (int i = 0; i < n; i++) { p.v[i] = i; }
        permuBan = p.encode(n);
        permuGen(&p, 0, &permus);
        std::sort(permus.begin(), permus.end());

        for (int i = 0; i < n; i++) { matrix[i] = 0; }
        for (int i = 0; i < n; i++) { matrixt[i] = 0; }

        int output = 0;
        if (n == 2 || n == 3) {
            output = 1;
        } else {
            // Fix first row and first col.
            for (int i = 0; i < n; i++) {
                setMatrix(0, i, i);
            }
            for (int i = 1; i < n; i++) {
                setMatrix(i, 0, i);
            }

            setMatrix(1, 1, 0);
            setMatrix(1, 2, 3);
            output += (n-3) * fillRow(1);

            setMatrix(1, 1, 2);
            setMatrix(1, 2, 3);
            output += (n-2) * (n-3) * fillRow(1);

            // setMatrix(1, 1, 2);
            setMatrix(1, 2, 0);
            output += (n-2) * fillRow(1);
        }

        result.set(output);
        for (int i = 1; i <= n-1; i++) {
            result.mul(static_cast<char>(i));
        }

        /*
        printMap("1/ ", m1);
        printMap("2/ ", m2);
        printMap("3/ ", m3);
        printMap("4/ ", m4);
        */
    }

    int fillRow(int row) {
        if (row == n-1) { return 1; }

        int ncirc = 0;
        int typ = 0;
        if (row == 2) {
            int row1[7];
            uint64_t r = matrix[1];
            for (int i = 0; i < n; i++) {
                row1[i] = getBitValue(r, i);
            }
            if (row1[1] == 0) {
                typ = 1;
                int peer = row1[2];
                int len = 1;
                while (peer != 2) {
                    peer = row1[peer];
                    len++;
                }
                if (len == n-2) {
                    ncirc = 1;
                } else {
                    ncirc = 2;
                }
            } else if (row1[2] == 3) {
                typ = 2;
                int peer = row1[3];
                int len = 1;
                // 0->1, 1->2, 2->3, so 3 has to go back to 0
                while (peer != 0) {
                    peer = row1[peer];
                    len++;
                }
                if (len == n-3) {
                    ncirc = 1;
                } else if (len > 1) {
                    ncirc = 2;
                }  else {
                    ncirc = 3; // still 2 circle, but a shorter one
                }
            } else if (row1[2] == 0) {
                typ = 3;
                int peer = row1[3];
                int len = 1;
                while (peer != 3) {
                    peer = row1[peer];
                    len++;
                }
                if (len == n-3) {
                    ncirc = 1;
                } else {
                    ncirc = 2;
                }
            }

            if (typ == 1) {
                if (ncirc == 1 && t1circ1 >= 0) return t1circ1;
                if (ncirc == 2 && t1circ2 >= 0) return t1circ2;
            } else if (typ == 2) {
                if (ncirc == 1 && t2circ1 >= 0) return t2circ1;
                if (ncirc == 2 && t2circ2 >= 0) return t2circ2;
                if (ncirc == 3 && t2circ2 >= 0) return t2circ3;
            } else if (typ == 3) {
                if (ncirc == 1 && t3circ1 >= 0) return t3circ1;
                if (ncirc == 2 && t3circ2 >= 0) return t3circ2;
            }
        }

        int ret = 0;
        uint64_t cur = matrix[row];
        int cut = row;
        if (row == 1) { cut += 2; } else { cut = 1; }
        setClearAfter(&cur, cut);
        vector<uint64_t>::iterator from = upper_bound(
                permus.begin(), permus.end(), cur);
        setFillAfter(&cur, cut);
        vector<uint64_t>::iterator to = upper_bound(
                from, permus.end(), cur);

        uint64_t conflict = 0;
        for (int j = 1; j < row; j++) {
            conflict |= matrix[j];
        }

        for (vector<uint64_t>::iterator it = from;
                it != to; it++) {
            if ((*it & conflict) != 0) continue;

            matrix[row] = *it;
            for (int j = cut; j < n; j++) {
                uint64_t set = getBitset(*it, j);
                setBitset(&matrixt[j], row, set);
            }
            ret += fillRow(row+1);
        }

        /*
        if (row <= 4) {
            if (row == 2) {
                printf("row %d: %d  typ=%d ncirc=%d\n",
                        row, ret, typ, ncirc);
                printFirstRows(matrix, n, 2);
                printf("\n");
            }
            if (row == 1) { mapInc(m1, ret); }
            if (row == 2) { mapInc(m2, ret); }
            if (row == 3) { mapInc(m3, ret); }
            if (row == 4) { mapInc(m4, ret); }
        }
        */

        if (row == 2) {
            if (typ == 1) {
                if (ncirc == 1) t1circ1 = ret;
                if (ncirc == 2) t1circ2 = ret;
            } else if (typ == 2) {
                if (ncirc == 1) t2circ1 = ret;
                if (ncirc == 2) t2circ2 = ret;
                if (ncirc == 3) t2circ3 = ret;
            } else if (typ == 3) {
                if (ncirc == 1) t3circ1 = ret;
                if (ncirc == 2) t3circ2 = ret;
            }
        }

        return ret;
    }

    int fillCol(int col) {
        int ret = 0;

        uint64_t cur = matrixt[col];
        int cut = col+1;
        setClearAfter(&cur, cut);
        vector<uint64_t>::iterator from = upper_bound(
                permus.begin(), permus.end(), cur);
        setFillAfter(&cur, cut);
        vector<uint64_t>::iterator to = upper_bound(
                from, permus.end(), cur);

        uint64_t conflict = 0;
        for (int j = 1; j < col; j++) {
            conflict |= matrixt[j];
        }

        for (vector<uint64_t>::iterator it = from;
                it != to; it++) {
            if ((*it & conflict) != 0) continue;

            matrixt[col] = *it;
            for (int j = cut; j < n; j++) {
                uint64_t set = getBitset(*it, j);
                setBitset(&matrix[j], col, set);
            }
            ret += fillRow(col+1);
        }
        return ret;
    }

    void writeOut() {
        fout = fopen("latin.out", "w"); assert(fout);
        result.print(fout);
        fprintf(fout, "\n");
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

