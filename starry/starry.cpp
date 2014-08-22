/*
ID: liulonn1
PROG: starry
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>

#include <vector>
using std::vector;

inline int _ind(int x, int y) {
    return (x << 7) | (y & 0x7f);
}

struct Bound {
    int minRow, maxRow;
    int minCol, maxCol;
    int _w, _h;
    int count;
    int code;

    void init(int r, int c) {
        count = 0;
        minRow = maxRow = r;
        minCol = maxCol = c;
    }

    int w() { return maxCol - minCol + 1; }
    int h() { return maxRow - minRow + 1; }
    int isDot() { return maxCol == minCol && maxRow == minRow; }

    void add(int r, int c) {
        count++;
        if (r < minRow) { minRow = r; }
        if (r > maxRow) { maxRow = r; }
        if (c < minCol) { minCol = c; }
        if (c > maxCol) { maxCol = c; }
    }

    int ind(int r, int c) {
        return _ind(minRow + r, minCol + c);
    }

    int ind1(int m, int r, int c) {
        switch (m) {
            case 1:
                return _ind(maxRow - r, minCol + c);
            case 2:
                return _ind(minRow + r, maxCol - c);
            case 3:
                return _ind(maxRow - r, maxCol - c);
            case 0: 
            default:
                return _ind(minRow + r, minCol + c);
        }
    }

    int ind2(int m, int r, int c) {
        return ind1(m, c, r);
    }

    void calcSize() { _w = w(); _h = h(); }
};

struct Prob {
    FILE * fin;
    FILE * fout;
    int w, h;

    int pic[128 * 128];
    Bound bounds[512];
    int outputChar[512];

    int ind(int x, int y) {
        return _ind(x, y);
    }

    void indr(int ind, int & x, int & y) {
        x = (ind >> 7) & 0x7f;
        y = ind & 0x7f;
    }

    bool validCord(int x, int y) {
        return x >= 0 && x < h && y >= 0 && y < w;
    }

    int get(int x, int y) {
        return pic[ind(x, y)];
    }
    void set(int x, int y, int v) {
        pic[ind(x, y)] = v;
    }

    void readIn() {
        fin = fopen("starry.in", "r"); assert(fin);
        int ret = fscanf(fin, "%d", &w); assert(ret == 1);
        ret = fscanf(fin, "%d", &h); assert(ret == 1);

        char buf[128];
        for (int i = 0; i < h; i++) {
            ret = fscanf(fin, "%s", buf);
            assert(ret == 1);

            for (int j = 0; j < w; j++) {
                if (buf[j] == '0') {
                    set(i, j, 0);
                } else {
                    set(i, j, -1);
                }
            }
        }
        fclose(fin);
    }

    void pour(int i, int j, int code) {
        Bound & bd = bounds[code];
        set(i, j, code);
        vector<int> stack;
        stack.push_back(ind(i, j));
        bd.init(i, j);
        bd.code = code;

        while (!stack.empty()) {
            int cur = stack.back();
            stack.pop_back();

            int row, col;
            indr(cur, row, col);
            // printf("- %d %d\n", row, col);
            
            for (int x = -1; x <= 1; x++) {
                for (int y = -1; y <= 1; y++) {
                    if (x == 0 && y == 0) continue;
                    int _r = row + x;
                    int _c = col + y;
                    if (!validCord(_r, _c)) continue;
                    int next = ind(_r, _c);
                    assert(next >= 0);

                    int px = pic[next];
                    if (px >= 0) continue;

                    bd.add(_r, _c);
                    pic[next] = code; // color this pixel
                    stack.push_back(next);
                }
            }
        }
    }

    void color() {
        int code = 1;
        char ch = 'a';

        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                int c = get(i, j);
                if (c >= 0) continue; // blank or colored
                // printf("new code: (%d, %d), %d, %d \n", i, j, c, code);

                pour(i, j, code);
                outputChar[code] = ch;
                ch++;

                for (int k = 1; k < code; k++) {
                    if (sameShape(code, k)) {
                        outputChar[code] = outputChar[k];
                        ch--;
                        break;
                    }
                }
                code++;
            }
        }
    }

    bool same1(Bound & b1, Bound & b2, int m) {
        int w = b1._w;
        int h = b1._h;
        
        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
                if (pic[b1.ind(i, j)] != b1.code) {
                    continue;
                }
                if (pic[b2.ind1(m, i, j)] != b2.code) {
                    return false;
                }
            }
        }

        return true;
    }

    bool same2(Bound & b1, Bound & b2, int m) {
        int w = b1._w;
        int h = b1._h;

        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
                if (pic[b1.ind(i, j)] != b1.code) {
                    continue;
                }
                if (pic[b2.ind2(m, i, j)] != b2.code) {
                    return false;
                }
            }
        }

        return true;
    }

    bool sameShape(int c1, int c2) {
        Bound & b1 = bounds[c1];
        Bound & b2 = bounds[c2];
        if (b1.isDot() && b2.isDot()) return true;
        if (b1.count != b2.count) return false;

        b1.calcSize();
        b2.calcSize();

        if (b1._w == b2._w && b1._h == b2._h) {
            for (int m = 0; m < 4; m++) {
                if (same1(b1, b2, m)) return true;
            }
        }

        if (b1._w == b2._h && b1._h == b2._w) {
            for (int m = 0; m < 4; m++) {
                if (same2(b1, b2, m)) return true;
            }
        }

        return false;
    }

    void writeOut() {
        color();

        fout = fopen("starry.out", "w"); assert(fout);

        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                int px = get(i, j);
                assert(px >= 0);
                char c = '0';
                if (px > 0) {
                    c = outputChar[px];
                }
                fprintf(fout, "%c", c);
            }
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

