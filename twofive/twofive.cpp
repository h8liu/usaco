/*
ID: liulonn1
PROG: twofive
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <map>

using std::map;

struct Line {
    char c[5];
    int n;

    void push(char _c) {
        c[n++] = _c;
    }

    void pop() {
        n--;
    }

    char last() {
        return c[n-1];
    }
};

struct Prob {
    FILE * fin;
    FILE * fout;

    Line lines[5];
    map<int, int> counts;

    char query;
    char sbuf[26];
    int target;
    int rrindex[25];
    int rindex[25];

    int code() {
        int ret = 0;
        for (int i = 0; i < 5; i++) {
            ret *= 6;
            ret += lines[i].n;
        }
        return ret;
    }
    
    void readIn() {
        fin = fopen("twofive.in", "r"); assert(fin);
        int ret = fscanf(fin, "%c", &query); assert(ret == 1);
        if (query == 'N') {
            ret = fscanf(fin, "%d", &target); assert(ret == 1);
            target--;
        } else {
            assert(query == 'W');
            ret = fscanf(fin, "%s", sbuf); assert(ret == 1);
            assert(strlen(sbuf) == 25);
        }
        fclose(fin);
    }

    void print() {
        for (int i = 0; i < 5; i++) {
            Line & line = lines[i];
            assert(line.n == 5);
            for (int j = 0; j < 5; j++) {
                printf("%c", line.c[j]);
            }
            printf(" ");
        }
        printf("\n");
    }

    void printpre() {
        for (int i = 0; i < 5; i++) {
            Line & line = lines[i];
            for (int j = 0; j < line.n; j++) {
                printf("%c", line.c[j]);
            }
            if (line.n != 5) {
                break;
            }
            printf(" ");
        }
        printf("\n");
    }

    void printpre2() {
        for (int i = 0; i < 5; i++) {
            Line & line = lines[i];
            for (int j = 0; j < line.n; j++) {
                printf("%c", line.c[j]);
            }
            if (line.n == 0) {
                break;
            }
            printf(" ");
        }
        printf("\n");
    }

    void printIndex() {
        for (int i = 0; i < 5; i++) {
            printf("%d ", lines[i].n);
        }
        printf("\n");
    }

    void precount(int cur) {
        if (cur == 5) {
            int c = code();

            if (c == 6 * 6 * 6 * 6 * 6 - 1) {
                counts[c] = 1;
                return;
            }   

            int count = 0;

            for (int i = 0; i < 5; i++) {
                Line & line = lines[i];
                if (line.n == 5) {
                    continue;
                }

                if (line.n == 0) {
                    line.n++;
                    int c2 = code();
                    assert(counts.find(c2) != counts.end());
                    count += counts[c2];
                    line.n--;
                    break;
                }

                if (i == 0 || line.n < lines[i-1].n) {
                    line.n++;
                    int c2 = code();
                    assert(counts.find(c2) != counts.end());
                    count += counts[c2];
                    line.n--;
                }
            }
            
            counts[c] = count;
            return;
        }
        
        if (cur == 0) {
            for (int i = 5; i >= 0; i--) {
                lines[0].n = i;
                precount(cur+1);
            }
        } else {
            Line & line = lines[cur-1];
            for (int i = line.n; i >= 0; i--) {
                lines[cur].n = i;
                precount(cur+1);
            }
        }
    }

    int findCount;
    int matchCount;
    int depth;

    void findCountAdd(int a) {
        // int was = findCount;
        findCount += a;
        /*
        if (was / 100000 != findCount / 100000) {
            printf("%d\n", findCount);
        }
        */
    }

    int eval() {
        assert(depth > 0);
    
        char last = 0;
        for (int i = 0; i < 5; i++) {
            if (lines[i].n == 0) {
                break;
            }
            char c = lines[i].last();
            if (c > last) {
                last = c;
            }
        }
        assert(last != 0);

        if ((int)(last - 'A') + 1 == depth) {
            int c = code();
            assert(counts.find(c) != counts.end());
            return counts[c]; 
        }

        return ecount('A', last);
    }

    int ecount(char first, char last) {
        char c = 0;
        for (char t = first; t < last; t++) {
            if (rindex[int(t - 'A')] >= 0) {
                continue;
            }
            c = t;
            break;
        }

        if (c == 0) {
            int c = code();
            assert(counts.find(c) != counts.end());
            int ret = counts[c];
            return ret;
        }

        int cind = int(c-'A');
        assert(rindex[cind] == -1);
        rindex[cind] = 0;

        int sum = 0;
        for (int i = 1; i < 5; i++) {
            Line & line = lines[i];
            if (line.n >= 5) {
                continue;
            }
            
            int col = line.n;

            if (line.n == 0) {
                if (c > lines[i-1].c[col]) {
                    line.push(c);
                    sum += ecount(c+1, last);
                    line.pop();
                }
                break;
            }

            if (line.n < lines[i-1].n) {
                if (c > lines[i-1].c[col] && c > lines[i].last()) {
                    line.push(c);
                    sum += ecount(c+1, last);
                    line.pop();
                }
            }
        }

        rindex[cind] = -1;
        return sum;
    }

    bool find(int row) {
        if (row == 5) {
            if (query == 'N') {
                if (findCount < target) {
                    findCountAdd(1);
                    return false;
                } else {
                    assert(findCount == target);
                    return true;
                }
            } else {
                assert(query == 'W');
                if (matchCount == 25) {
                    return true;
                } else {
                    findCountAdd(1);
                    return false;
                }
            }
        }

        if (depth > 0) {
            int nsub = eval();
            // printf("%d %d: ", findCount, nsub);
            // printpre();
            if (nsub == 0) {
                return false;
            }

            if (query == 'N') {
                int delta = target - findCount;
                if (delta >= nsub) {
                    // printf("del=%d\n", delta);
                    findCountAdd(nsub);
                    return false; // skipping this branch
                }
            } else if (query == 'W') {
                if (matchCount < depth) {
                    findCountAdd(nsub);
                    return false;
                }
            }
        }

        depth++;
        int col = lines[row].n;
        // printf("%d %d %d\n", depth, row, col);
        
        char start = 'A';
        if (col > 0) {
            char c = lines[row].c[col-1] + 1;
            if (c > start) {
                start = c;
            }
        }
        if (row > 0) {
            char c = lines[row-1].c[col] + 1;
            if (c > start) {
                start = c;
            }
        }

        for (char c = start; c < 'Z'; c++) {
            int ind = (int)(c-'A');
            if (rindex[ind] >= 0) {
                continue;
            }

            lines[row].push(c);
            rindex[ind] = row * 5 + col;
            bool match = rindex[ind] == rrindex[ind];
            if (match) {
                matchCount++;
            }
            
            if (lines[row].n == 5) {
                if (find(row + 1)) { return true; }
            } else {
                if (find(row)) { return true; }
            }

            lines[row].pop();
            if (match) {
                matchCount--;
            }
            rindex[ind] = -1;
        }

        depth--;

        return false;
    }

    void writeOut() {
        for (int i = 0; i < 5; i++) {
            lines[i].n = 0;
        }
        precount(0);
        for (int i = 0; i < 5; i++) {
            lines[i].n = 0;
        }

        fout = fopen("twofive.out", "w"); assert(fout);

        if (query == 'N') {
            assert(target < counts[0]);
            for (int i = 0; i < 25; i++) {
                rindex[i] = -1;
            }
            findCount = 0;
            matchCount = 0;
            depth = 0;
            bool ret = find(0);
            assert(ret);
            
            for (int i = 0; i < 5; i++) {
                Line & line = lines[i];
                assert(line.n == 5);
                for (int j = 0; j < 5; j++) {
                    fprintf(fout, "%c", line.c[j]);
                }
            }
            fprintf(fout, "\n");
        } else {
            assert(query == 'W');

            for (int i = 0; i < 25; i++) {
                rrindex[int(sbuf[i] - 'A')] = i;
            }
            
            for (int i = 0; i < 25; i++) {
                rindex[i] = -1;
            }
            findCount = 0;
            matchCount = 0;
            depth = 0;
            bool ret = find(0);
            assert(ret);

            fprintf(fout, "%d\n", findCount+1);
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

