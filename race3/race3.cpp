/*
ID: liulonn1
PROG: race3
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

struct Node {
    int id;
    vector<int> tos;
    vector<int> froms;
    bool mark;
    bool mark2;
};

struct Prob {
    FILE * fin;
    FILE * fout;

    Node nodes[500];
    int n;
    
    void readIn() {
        fin = fopen("race3.in", "r"); assert(fin);

        n = 0;
        int ret;
        while (true) {
            int x;
            ret = fscanf(fin, "%d", &x); assert(ret == 1);
            if (x == -2) {
                n++;
            } else if (x == -1) {
                break;
            } else {
                nodes[n].tos.push_back(x);
                nodes[x].froms.push_back(n);
            }
        }

        fclose(fin);
    }

    bool isCritical(int x) {
        if (x == 0 || x == n - 1) {
            return true;
        }

        deque<int> q;
        q.push_back(0);

        for (int i = 0; i < n; i++) {
            nodes[i].mark = false;
        }
        nodes[0].mark = true;

        bool ret = true;
        while (!q.empty()) {
            int th = q.back();
            q.pop_back();
            
            Node & nd = nodes[th];
            for (vector<int>::iterator it = nd.tos.begin();
                    it != nd.tos.end(); it++) {
                int next = *it;
                if (next == x) {
                    continue; // will never touch x;
                }

                if (next == n - 1) {
                    ret = false;
                }

                if (nodes[next].mark) {
                    continue;
                }
                
                nodes[next].mark = true;
                q.push_back(*it);
            }
        }

        return ret;
    }

    bool isChoke(int x) {
        deque<int> q;

        // mark this node
        for (int i = 0; i < n; i++) {
            nodes[i].mark2 = false;
        }

        q.push_back(x);
        while (!q.empty()) {
            int th = q.back();
            q.pop_back();
            
            Node & n = nodes[th];
            for (vector<int>::iterator it = n.tos.begin();
                    it != n.tos.end(); it++) {
                int next = *it;
                if (nodes[next].mark2) {
                    continue;
                }
                
                nodes[next].mark2 = true;
                if (nodes[next].mark) {
                    return false;
                }
                q.push_back(*it);
            }
        }

        return true;
    }

    void writeOut() {
        vector<int> criticals;
        vector<int> chokes;

        for (int i = 1; i < n-1; i++) {
            if (isCritical(i)) {
                criticals.push_back(i);
                if (isChoke(i)) {
                    chokes.push_back(i);
                }
            }
        }

        fout = fopen("race3.out", "w"); assert(fout);
        
        fprintf(fout, "%ld", criticals.size());
        for (vector<int>::iterator it = criticals.begin();
                it != criticals.end(); it++) {
            fprintf(fout, " %d", *it);
        }
        fprintf(fout, "\n");

        fprintf(fout, "%ld", chokes.size());
        for (vector<int>::iterator it = chokes.begin();
                it != chokes.end(); it++) {
            fprintf(fout, " %d", *it);
        }
        fprintf(fout, "\n");

        fclose(fout);
    }
};

Prob prob;

int main() {
    prob.readIn();
    prob.writeOut();
    return 0;
}

