/*
ID: liulonn1
PROG: fence6
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>

struct NodeSig {
    unsigned char buf[13];
    int nbit;
    int id;

    void clear() {
        for (int i = 0; i < 13; i++) {
            buf[i] = 0;
        }
        nbit = 0;
        id = -1;
    }
    
    void set(int i) {
        int off = i % 8;
        int ind = i / 8;
        unsigned char bit = (unsigned char)(0x1 << off);
        if ((buf[ind] & bit) == 0) {
            nbit++;
        }
        buf[ind] |= bit;
    }

    bool get(int i) {
        int off = i % 8;
        int ind = i / 8;
        unsigned char bit = (unsigned char)(0x1 << off);
        if ((buf[ind] & bit) == 0) {
            return false;
        }
        return true;
    }

    int cmp(NodeSig *other) {
        for (int i = 0; i < 13; i++) {
            if (buf[i] < other->buf[i]) {
                return -1;
            } else if (buf[i] > other->buf[i]) {
                return 1;
            }
        }
        return 0;
    }

    bool eq(NodeSig *other) {
        return cmp(other) == 0;
    }

    bool hasCommon(NodeSig *other) {
        for (int i = 0; i < 13; i++) {
            if ((buf[i] & other->buf[i]) != 0) {
                return true;
            }
        }
        return false;
    }
};

const int FARAWAY = 1000 * 1000;

struct Prob {
    FILE * fin;
    FILE * fout;

    int n;

    int len[101];

    NodeSig leftSigs[101];
    NodeSig rightSigs[101];

    int v;
    NodeSig * nodeSigs[300];
    int sdist[300][300];
    int dist[300][300];
    int edge[300][300];
    
    void readIn() {
        fin = fopen("fence6.in", "r"); assert(fin);
        int ret;
        ret = fscanf(fin, "%d", &n); assert(ret == 1);
        assert(n >= 1 && n <= 100);

        for (int i = 1; i <= n; i++) {
            leftSigs[i].clear();
            rightSigs[i].clear();
        }

        for (int i = 0; i < 300; i++) {
            for (int j = 0; j < 300; j++) {
                dist[i][j] = FARAWAY;
                sdist[i][j] = FARAWAY;
                edge[i][j] = 0;
            }
        }

        for (int i = 0; i < n; i++) {
            int id;
            int thislen;
            int nleft, nright;
            ret = fscanf(fin, "%d %d %d %d", &id, &thislen, &nleft, &nright); 
            assert(ret == 4);
            len[id] = thislen;

            int otherid;
        
            for (int j = 0; j < nleft; j++) {
                ret = fscanf(fin, "%d", &otherid); assert(ret == 1);
                leftSigs[id].set(otherid);
            }
            for (int j = 0; j < nright; j++) {
                ret = fscanf(fin, "%d", &otherid); assert(ret == 1);
                rightSigs[id].set(otherid);
            }
            
            assert(!leftSigs[id].hasCommon(&rightSigs[id]));

            leftSigs[id].set(id);
            rightSigs[id].set(id);
        }

        fclose(fin);

        v = 0;
        // cluster the nodes
        for (int i = 1; i <= n; i++) {
            for (int j = 0; j < v; j++) {
                if (nodeSigs[j]->eq(&leftSigs[i])) {
                    leftSigs[i].id = nodeSigs[j]->id;
                    break;
                }
            }
            if (leftSigs[i].id < 0) {
                leftSigs[i].id = v;
                nodeSigs[v] = &leftSigs[i];
                v++;
            }
            
            for (int j = 0; j < v; j++) {
                if (nodeSigs[j]->eq(&rightSigs[i])) {
                    rightSigs[i].id = nodeSigs[j]->id;
                    break;
                }
            }
            if (rightSigs[i].id < 0) {
                rightSigs[i].id = v;
                nodeSigs[v] = &rightSigs[i];
                v++;
            }
            
            int left = leftSigs[i].id;
            int right = rightSigs[i].id;
            dist[left][right] = len[i];
            dist[right][left] = len[i];
            edge[right][left] = i;
            edge[left][right] = i;
        }
    }

    void writeOut() {
        /*
        printf("%d nodes\n", v);
        
        for (int i = 0; i < v; i++) {
            for (int j = 0; j < v; j++) {
                if (edge[i][j] > 0) {
                    printf("edge %d: %d - %d: %d\n", 
                            edge[i][j],
                            i, j, dist[i][j]);
                }
            }
        }
        */

        int min = FARAWAY;

        for (int i = 1; i < n; i++) {
            int left = leftSigs[i].id;
            int right = rightSigs[i].id;

            int hold = dist[left][right];
            dist[left][right] = FARAWAY;
            dist[right][left] = FARAWAY;

            if (len[i] < min) {
                int d = find(left, right, min-len[i]) + len[i];
                if (d < min) {
                    min = d;
                }
            }

            dist[left][right] = hold;
            dist[right][left] = hold;
        }

        fout = fopen("fence6.out", "w"); assert(fout);
        fprintf(fout, "%d\n", min);
        fclose(fout);
    }

    int find(int left, int right, int atleast) {
        int d[300];
        int closed[300];

        for (int i = 0; i < v; i++) {
            d[i] = FARAWAY;
            closed[i] = false;
        }

        d[left] = 0;
        
        while (true) {
            int imin = -1;
            for (int i = 0; i < v; i++) {
                if (closed[i]) {
                    continue;
                }
                
                if (imin < 0 || d[i] < d[imin]) {
                    imin = i;
                }
            }

            if (imin < 0) {
                // all closed
                break;
            }
            
            int dmin = d[imin];
            if (dmin >= atleast) {
                return atleast;
            }
            if (imin == right) {
                return dmin;
            }

            // extend now
            for (int i = 0; i < v; i++) {
                if (closed[i]) {
                    continue;
                }

                if (dist[i][imin] + dmin < d[i]) {
                    d[i] = dist[i][imin] + dmin;
                }
            }

            closed[imin] = true;
        }

        // not reachable
        return atleast;
    }
};

Prob prob;

int main() {
    prob.readIn();
    prob.writeOut();
    return 0;
}

