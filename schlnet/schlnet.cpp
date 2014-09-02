/*
ID: liulonn1
PROG: schlnet
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>

#include <vector>
#include <deque>
using std::vector;
using std::deque;

struct Sch {
    bool out;
    bool before;
    bool after;
    bool head;
    bool tail;
    vector<int> sends;
    vector<int> recvs;
};

struct Prob {
    FILE * fin;
    FILE * fout;
    int n;
    Sch schs[100];
    
    void readIn() {
        fin = fopen("schlnet.in", "r"); assert(fin);
        int ret = fscanf(fin, "%d", &n); assert(ret == 1);

        for (int i = 0; i < n; i++) {
            Sch & s = schs[i];
            s.out = false;
            s.head = false;
            s.tail = false;
            s.before = false;
            s.after = false;
        }

        for (int i = 0; i < n; i++) {
            Sch & s = schs[i];

            while (true) {
                int v;
                ret = fscanf(fin, "%d", &v); assert(ret == 1);
                if (v == 0) { break; }
                v--;

                s.sends.push_back(v);
                schs[v].recvs.push_back(i);
            }
        }

        fclose(fin);
    }

    int touch(int v, bool rev=false) {
        assert(!schs[v].out);

        for (int i = 0; i < n; i++) {
            schs[i].before = false;
            schs[i].after = false;
        }
            
        schs[v].before = true;
        schs[v].after = true;
        
        deque<int> q;
        q.push_back(v);
        while (!q.empty()) {
            int t = q.front();
            q.pop_front();

            Sch & s = schs[t];
            for (vector<int>::iterator it = s.recvs.begin();
                    it != s.recvs.end(); it++) {
                if (rev && schs[*it].out) {
                    continue;
                }
                if (schs[*it].before) {
                    continue;
                }
                q.push_back(*it);
                schs[*it].before = true;
            }
        }

        q.clear();
        q.push_back(v);
        while (!q.empty()) {
            int t = q.front();
            q.pop_front();

            Sch & s = schs[t];
            for (vector<int>::iterator it = s.sends.begin();
                    it != s.sends.end(); it++) {
                if (!rev && schs[*it].out) {
                    continue;
                }
                if (schs[*it].after) {
                    continue;
                }
                q.push_back(*it);
                schs[*it].after = true;
            }
        }

        for (int i = 0; i < n; i++) {
            if (!rev && schs[i].before && !schs[i].after) {
                return i;
            }
            if (rev && schs[i].after && !schs[i].before) {
                return i;
            }
        }

        return v;
    }

    int oneHead;
    int oneTail;

    int taska(int & b) {
        int ret = 0;

        while (true) {
            int found = -1;
            for (int i = 0; i < n; i++) {
                if (!schs[i].out) {
                    found = i;
                    break;
                }
            }
            if (found < 0) {
                break;
            }

            int last = found;
            
            // find a node that is a head
            while (true) {
                int t = touch(last);
                if (t == last) {
                    break;
                }
                last = t;
            }

            ret++;
            for (int i = 0; i < n; i++) {
                if (schs[i].after) {
                    schs[i].out = true;
                }
                if (schs[i].before && schs[i].after) {
                    schs[i].head = true;
                }
            }
        }

        for (int i = 0; i < n; i++) {
            schs[i].out = false;
        }

        b = 0;
        while (true) {
            int found = -1;
            for (int i = 0; i < n; i++) {
                if (!schs[i].out) {
                    found = i;
                    break;
                }
            }
            if (found < 0) { break; }

            int last = found;
            while (true) {
                int t = touch(last, true);
                if (t == last) { break; }
                last = t;
            }

            b++;
            for (int i = 0; i < n; i++) {
                if (schs[i].before) {
                    schs[i].out = true;
                }
                if (schs[i].before && schs[i].after) {
                    schs[i].tail = true;
                }
            }
        }

        return ret;
    }

    bool allInOne() {
        for (int i = 0; i < n; i++) {
            if (schs[i].head && schs[i].tail) {
                continue;
            }
            return false;
        }
        return true;
    }

    void writeOut() {
        int a, b;
        a = taska(b);
        // printf("a=%d b=%d\n", a, b);
        if (a > b) { b = a; }
        if (a == 1 && b == 1 && allInOne()) { b = 0; }

        fout = fopen("schlnet.out", "w"); assert(fout);
        fprintf(fout, "%d\n", a);
        fprintf(fout, "%d\n", b);
        fclose(fout);
    }
};

Prob prob;

int main() {
    prob.readIn();
    prob.writeOut();
    return 0;
}

