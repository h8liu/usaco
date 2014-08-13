/*
ID: liulonn1
PROG: milk6
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>

#include <stdint.h>

#include <vector>
using std::vector;

struct Edge {
    int id;
    int from;
    int to;
    int64_t weight;
    int64_t used;
    bool avoid;
    bool checked;
};

struct Node {
    vector<Edge *> ins;
    vector<Edge *> outs;
    bool visited;
    int last;
    int lastExtend;
    Edge * lastEdge;
    bool lastIsIn;
    int id;
};

// const int64_t MASK = ((1 << 10) - 1);

struct Prob {
    FILE * fin;
    FILE * fout;
    int n, m;

    Node nodes[32];
    vector<Edge *> iedges;

    void readIn() {
        fin = fopen("milk6.in", "r"); assert(fin);
        int ret = fscanf(fin, "%d %d", &n, &m);
        assert(ret == 2);

        int s, e, c;
        for (int i = 0; i < m; i++) {
            ret = fscanf(fin, "%d %d %d", &s, &e, &c);
            s--;
            e--;
            Edge * edge = new(Edge);
            edge->id = i;
            edge->from = s;
            edge->to = e;
            edge->weight = (c << 10) + 1;
            edge->used = 0;
            edge->avoid = false;
            edge->checked = false;
            
            nodes[e].ins.push_back(edge);
            nodes[s].outs.push_back(edge);
            iedges.push_back(edge);
        }

        fclose(fin);

        for (int i = 0; i < n; i++) {
            nodes[i].id = i;
        }
    }

    int64_t flow() {
        for (int i = 0; i < n; i++) {
            nodes[i].visited = false;
        }

        Node * start = &nodes[0];
        start->last = -1;
        start->visited = true;
        vector<Node *> q;
        q.push_back(start);

        int ptr = 0;
        bool reached = false;
        while (ptr < int(q.size())) {
            Node * cur = q[ptr];
            if (cur->id == n-1) {
                reached = true;
                break;
            }

            for (vector<Edge *>::iterator it = cur->ins.begin();
                    it != cur->ins.end(); it++) {
                Edge * e = *it;
                if (e->avoid) continue;
                if (e->used == 0) continue;
                Node * node = &nodes[e->from];
                if (node->visited) continue;

                node->visited = true;
                node->last = ptr;
                node->lastExtend = e->used;
                node->lastEdge = e;
                node->lastIsIn = true;
                q.push_back(node);
                if (node->id == n-1) {
                    ptr = int(q.size()-1);
                    reached = true;
                    goto out;
                }
            }

            for (vector<Edge *>::iterator it = cur->outs.begin();
                    it != cur->outs.end(); it++) {
                Edge * e = *it;
                if (e->avoid) continue;
                if (e->used >= e->weight) continue;
                Node * node = &nodes[e->to];
                if (node->visited) continue;

                node->visited = true;
                node->last = ptr;
                node->lastExtend = e->weight - e->used;
                node->lastEdge = e;
                node->lastIsIn = false;
                q.push_back(node);

                if (node->id == n-1) {
                    ptr = int(q.size()-1);
                    reached = true;
                    goto out;
                }
            }

            ptr++;
        }

out:
        if (!reached) {
            return 0;
        }

        int64_t minExtend = 0;
        int startPtr = ptr; // hold it
        while (ptr >= 0) {
            Node * cur = q[ptr];
            if (cur->id == 0) break;
            ptr = cur->last;
            assert(cur->lastExtend > 0);
            if (minExtend == 0 || minExtend > cur->lastExtend) {
                minExtend = cur->lastExtend;
            }
        }
        
        assert(minExtend > 0);
        ptr = startPtr; // restore

        while (ptr >= 0) {
            Node * cur = q[ptr];
            if (cur->id == 0) break;
            ptr = cur->last;
            if (cur->lastIsIn) {
                assert(cur->lastEdge->used >= minExtend);
                cur->lastEdge->used -= minExtend;
            } else {
                assert(cur->lastEdge->weight - cur->lastEdge->used >= minExtend);
                cur->lastEdge->used += minExtend;
            }
        }

        return minExtend;
    }

    int64_t calcSum() {
        // clear usage
        for (int i = 0; i < m; i++) {
            iedges[i]->used = 0;
        }

        int64_t sum = 0;
        while (true) {
            int inc = flow();
            if (inc == 0) break;
            sum += inc;
        }

        return sum;
    }

    void pickSet(int64_t sum) {
        int64_t cur = sum;
        for (int i = 0; i < m; i++) {
            Edge * e = iedges[i];
            if (e->checked) {
                continue;
            }
            
            int64_t wsum = e->weight;
            e->avoid = true;
            for (int j = i + 1; j < m; j++) {
                Edge * e2 = iedges[j];
                if (e2->from == e->from && e2->to == e->to) {
                    e2->avoid = true;
                    e2->checked = true;
                    wsum += e2->weight;
                }
            }

            int64_t sumNow = calcSum();
 
            if (sumNow < cur && sumNow + wsum == cur) {
                /*
                printf("%d %lld:%lld %lld:%lld\n", i+1, 
                        sumNow >> 10, sumNow & ((1 << 10) - 1), 
                        cur >> 10, cur & ((1 << 10) - 1));
                */

                cur = sumNow;
                if (cur == 0) {
                    return;
                }
            } else {
                e->avoid = false;
                for (int j = i + 1; j < m; j++) {
                    Edge * e2 = iedges[j];
                    if (e2->from == e->from && e2->to == e->to) {
                        e2->avoid = false;
                    }
                }
            }
        }
    }

    void writeOut() {
        int64_t sum = calcSum();
        pickSet(sum);

        int64_t nedge = sum & ((1 << 10) - 1);
        sum >>= 10;
        fout = fopen("milk6.out", "w"); assert(fout);
        fprintf(fout, "%lld %lld\n", sum, nedge);

        for (int i = 0; i < m; i++) {
            if (iedges[i]->avoid) {
                fprintf(fout, "%d\n", i + 1);

                /*
                Edge *e = iedges[i];
                printf("%d %d->%d %lld:%lld\n", i + 1, e->from+1, e->to+1, 
                        e->weight >> 10, e->weight & MASK);
                */
            }
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

