/*
ID: liulonn1
PROG: theme
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>

#include <stdint.h>
#include <map>
#include <deque>
#include <vector>
using std::map;
using std::deque;
using std::vector;

struct Parts {
    vector<int> ps;
    int len;
};

struct Prob {
    FILE * fin;
    FILE * fout;

    int n;

    int notes[5000];
    int d[5000];

    int longest;
    int longFirst, longLast;
    
    void readIn() {
        fin = fopen("theme.in", "r"); assert(fin);
        int ret = fscanf(fin, "%d", &n); assert(ret == 1);
        for (int i = 0; i < n; i++) {
            ret = fscanf(fin, "%d", &notes[i]); assert(ret == 1);
        }
        fclose(fin);

        // we only care about the delta
        for (int i = 0; i < n-1; i++) {
            d[i] = notes[i+1] - notes[i] + 100; // always positive
        }
    }

    deque<Parts *> pres;

    void setup() {
        longest = 0;
        map<int, Parts *> pmap;

        for (int i = 0; i < n-1; i++) {
            int delta = d[i];
            map<int, Parts *>::iterator it = pmap.find(delta);
            if (it == pmap.end()) {
                // not found, lets add one
                Parts * ps = new Parts();
                ps->len = 2;
                ps->ps.push_back(i);
                pmap[delta] = ps;
            } else {
                Parts * ps = it->second;
                ps->ps.push_back(i);
            }
        }

        for (map<int, Parts *>::iterator it = pmap.begin();
                it != pmap.end(); it++) {
            Parts * ps = it->second;
            if (ps->ps.size() <= 1) {
                // single occurance
                delete ps;
            } else {
                // multiple occurance, a valid counting prefix
                pres.push_back(ps);
                saveLongest(ps);
            }
        }
    }

    void saveLongest(Parts *ps) {   
        if (ps->ps.back() - ps->ps.front() >= ps->len && longest < ps->len) {
            longest = ps->len; // extend the length;
            longFirst = ps->ps.front() - longest + 2;
            longLast = ps->ps.back() - longest + 2;
        }
    }

    void search() {
        while (!pres.empty()) {
            Parts * cur = pres.front();
            pres.pop_front();
            
            map<int, Parts *> pmap;
            for (vector<int>::iterator it = cur->ps.begin();
                    it != cur->ps.end(); it++) {
                int pos = (*it) + 1;
                if (pos >= n - 1) { continue; }
                int delta = d[pos];
                map<int, Parts*>::iterator found = pmap.find(delta);
                if (found == pmap.end()) {
                    Parts * ps = new Parts();
                    ps->len = cur->len + 1;
                    ps->ps.push_back(pos);
                    pmap[delta] = ps;
                } else {
                    Parts * ps = found->second;
                    ps->ps.push_back(pos);
                }
            }

            for (map<int, Parts *>::iterator it = pmap.begin();
                    it != pmap.end(); it++) {
                Parts * ps = it->second;
                if (ps->ps.size() <= 1) {
                    delete ps;
                } else {
                    pres.push_back(ps);
                    saveLongest(ps);
                }
            }

            delete cur;
        }
    }
    
    void printLongest() {
        for (int i = 0; i < longest; i++) {
            printf("%d ", notes[longFirst+i]);
        }
        printf("\n");

        for (int i = 0; i < longest; i++) {
            printf("%d ", notes[longLast+i]);
        }
        printf("\n");
    }

    void writeOut() {
        setup();
        search();
 
        if (longest < 5) { longest = 0; }
        fout = fopen("theme.out", "w"); assert(fout);
        fprintf(fout, "%d\n", longest);
        fclose(fout);
    }
};

Prob prob;

int main() {
    prob.readIn();
    prob.writeOut();
    return 0;
}

