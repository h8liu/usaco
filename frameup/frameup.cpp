/*
ID: liulonn1
PROG: frameup
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <stdint.h>

#include <vector>
#include <deque>
#include <algorithm>
using std::vector;
using std::deque;

struct Line {
    char c[30];

    bool operator < (const Line & other) const {
        int ret = strcmp(c, other.c);
        return ret < 0;
    }
};

struct Frame {
    char c;
    bool valid;
    int rowMin, rowMax;
    int colMin, colMax;
    int nabove;
    uint32_t below;
    bool removed;
};

struct Prob {
    FILE * fin;
    FILE * fout;

    Line lines[30];
    Frame frames[26];
    int h, w;
    
    void readIn() {
        fin = fopen("frameup.in", "r"); assert(fin);
        int ret = fscanf(fin, "%d %d", &h, &w); assert(ret == 2);
        
        for (int i = 0; i < h; i++) {
            ret = fscanf(fin, "%s", lines[i].c); assert(ret == 1);
        }
        fclose(fin);
    }

    void border(char c, int j) {
        assert(c != '.');
        int index = int(c - 'A');
        if (index == j) { return; }
        uint32_t bit = ((uint32_t)(0x1)) << j;
        uint32_t & below = frames[index].below;
        if ((below & bit) != bit) {
            // not counted yet
            below |= bit;
            frames[j].nabove++;
        }
    }

    deque<char> order;
    vector<Line> solutions;
    vector<Frame *> valids;

    void search(int left) {
        if (left == 0) {
            Line sol;
            int i = 0;
            for (deque<char>::iterator it = order.begin();
                    it != order.end(); it++) {
                sol.c[i++] = *it;
            }
            sol.c[i] = '\0';
            solutions.push_back(sol);
            return;
        }

        for (vector<Frame *>::iterator it = valids.begin();
                it != valids.end(); it++) {
            Frame * frame = *it;
            assert(frame->valid);
            if (frame->removed) continue;

            if (frame->nabove == 0) {
                // let's remove this frame for now
                frame->removed = true;
                order.push_front(frame->c);
                for (int j = 0; j < 26; j++) {
                    uint32_t bit = (uint32_t(0x1)) << j;
                    if ((frame->below & bit) == bit) {
                        assert(frames[j].nabove > 0);
                        frames[j].nabove--;
                    }
                }

                search(left - 1);

                // now let's restore this frame
                order.pop_front();
                frame->removed = false;
                for (int j = 0; j < 26; j++) {
                    uint32_t bit = (uint32_t(0x1)) << j;
                    if ((frame->below & bit) == bit) {
                        frames[j].nabove++;
                    }
                }
            }
        }
    }

    void writeOut() {
        for (int i = 0; i < 26; i++) {
            bzero(&frames[i], sizeof(struct Frame));
            frames[i].c = char('A' + i);
        }

        for (int row = 0; row < h; row++) {
            char * line = lines[row].c;
            for (int col = 0; col < w; col++) {
                char c = line[col];
                if (c == '.') {
                    continue;
                }
                int index = int(c - 'A');
                assert(index >= 0 && index < 26); // from 'A' to 'Z'
                Frame & frame = frames[index];
                if (frame.valid) {
                    if (row > frame.rowMax) frame.rowMax = row;
                    if (row < frame.rowMin) frame.rowMin = row;
                    if (col > frame.colMax) frame.colMax = col;
                    if (col < frame.colMin) frame.colMin = col;
                } else {
                    frame.valid = true;
                    frame.rowMax = row;
                    frame.rowMin = row;
                    frame.colMax = col;
                    frame.colMin = col;
                }
            }
        }

        for (int i = 0; i < 26; i++) {
            Frame & frame = frames[i];
            if (!frame.valid) { continue; }
            valids.push_back(&frame);

            assert(frame.rowMin <= frame.rowMax - 2);
            assert(frame.colMin <= frame.colMax - 2);
            for (int j = frame.rowMin; j <= frame.rowMax; j++) {
                border(lines[j].c[frame.colMin], i);
                border(lines[j].c[frame.colMax], i);
            }

            for (int j = frame.colMin + 1; j <= frame.colMax - 1; j++) {
                border(lines[frame.rowMin].c[j], i);
                border(lines[frame.rowMax].c[j], i);
            }
        }

        search(int(valids.size()));
        std::sort(solutions.begin(), solutions.end());

        fout = fopen("frameup.out", "w"); assert(fout);

        for (vector<Line>::iterator it = solutions.begin();
                it != solutions.end(); it++) {
            Line & line = *it;
            fprintf(fout, "%s\n", line.c);
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

