/*
ID: liulonn1
PROG: telecow
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>

#include <deque>

using std::deque;

struct Node {
    int edges[100];
    int nedge;
    int last; // for tracing back

    int in;
    int out;

    int id;
    bool removed;

    void addEdge(int node) {
        edges[nedge] = node;
        nedge++;
    }
};

struct Prob {
    FILE * fin;
    FILE * fout;

    int n, m, c1, c2;
    Node nodes[100];

    bool shortcut;
    
    void readIn() {
        shortcut = false;

        fin = fopen("telecow.in", "r"); assert(fin);
        int ret = fscanf(fin, "%d %d %d %d", &n, &m, &c1, &c2);
        assert(ret == 4);
        c1--; c2--;

        for (int i = 0; i < n; i++) {
            nodes[i].nedge = 0;
            nodes[i].removed = false;
            nodes[i].id = i;
        }

        for (int i = 0; i < m; i++) {
            int n1, n2;
            ret = fscanf(fin, "%d %d", &n1, &n2);
            assert(ret == 2);
            assert(n1 != n2);
            n1--; n2--;
            
            if ((n1 == c1 && n2 == c2) || (n1 == c2 && n2 == c1)) {
                shortcut = true;
            }
            nodes[n1].addEdge(n2);
            nodes[n2].addEdge(n1);
        }

        fclose(fin);
    }

    bool findPath() {
        for (int i = 0; i < n; i++) {
            nodes[i].last = -1;
        }

        deque<Node *> q;
        
        Node * node = &nodes[c1];
        node->last = c1; // self pointing

        for (int i = 0; i < node->nedge; i++) {
            int to = node->edges[i];
            Node * next = &nodes[to];
            if (next->removed || next->last >= 0) {
                continue;
            }
            
            if (next->id == c2) {
                // found dest already
                if (next->in < 0) {
                    next->in = c1; // mark this direct link
                    return true;
                }
            } else {
                // a normal node
                if (next->in != c1) {
                    next->last = c1;
                    q.push_back(next);
                }
            }
        }

        while (!q.empty()) {
            Node * node = q.front();
            q.pop_front();
            int from = node->id;
            assert(from != c1 && from != c2);

            if (node->out < 0 || node->last == node->out) {
                if (node->out < 0) {
                    assert(node->in < 0);
                }
                for (int i = 0; i < node->nedge; i++) {
                    int to = node->edges[i];
                    Node * next = &nodes[to];
                    if (next->removed || next->last >= 0) {
                        continue;
                    }

                    if (next->id == c2) {
                        // found dest.
                        next->last = from;
                        flipBack();
                        return true;
                    } else {
                        next->last = from;
                        q.push_back(next);
                    }
                }
            } else {
                assert(node->out >= 0 && node->in >= 0);
                assert(node->last != node->out);
                int to = node->in;
                Node * next = &nodes[to];
                assert(!next->removed);
                if (next->last < 0) {
                    assert(next->out == from);
                    next->last = from;
                    q.push_back(next);
                }
            }
        }

        return false;
    }

    void flipBack() {
        Node * node = &nodes[c2];

        while (1) {
            Node * pre = &nodes[node->last];
            if (pre->id == c1) {
                return;
            }

            if (pre->in < 0) {
                assert(pre->out < 0);
                pre->out = node->id;
                pre->in = pre->last;
            } else if (pre->out == pre->last) {
                pre->out = node->id;
                // pre->in no change
            } else {
                pre->in = pre->last;
                // pre->out no change
            }
            
            node = pre;
        }
    }

    int minCut() {
        assert(c1 != c2);

        for (int i = 0; i < n; i++) {
            nodes[i].in = -1;
            nodes[i].out = -1;
        }

        int ret = 0;
        while (findPath()) {
            ret++;
        }
        return ret;
    }

    int tryRemove() {
        int mc = minCut();
        int ret = 0;

        for (int i = 0; i < n; i++) {
            if (i == c1 || i == c2) {
                continue;
            } 
            nodes[i].removed = true;

            int newMc = minCut();
            if (newMc < mc) {
                mc = newMc;
                ret++;
                if (mc == 0) {
                    return ret;
                }

                continue;
            }

            nodes[i].removed = false;
        }
        
        assert(false);
        return 0; // c1 and c2 are connected
    }


    void writeOut() {
        // printf("c1=%d c2=%d\n", c1, c2);

        fout = fopen("telecow.out", "w"); assert(fout);

        if (shortcut) {
            fprintf(fout, "0\n");
        } else {
            int res = tryRemove();
            fprintf(fout, "%d\n", res);
            int nprint = 0;
            for (int i = 0; i < n; i++) {
                if (nodes[i].removed) {
                    if (nprint > 0) {
                        fprintf(fout, " ");
                    }
                    fprintf(fout, "%d", i+1);
                    nprint++;
                }
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

