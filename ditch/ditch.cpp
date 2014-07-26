/*
ID: liulonn1
PROG: ditch
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>

struct Prob {
    FILE * fin;
    FILE * fout;
    
    int m;
    int n;
    
    int d[300][300];
    int f[300][300];

    void readIn() {
        fin = fopen("ditch.in", "r"); assert(fin);
        int read = fscanf(fin, "%d %d", &n, &m);
        assert(read == 2);
        
        int s, e, c;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < m; j++) {
                d[i][j] = 0;
            }
        }

        for (int i = 0; i < n; i++) {
            read = fscanf(fin, "%d %d %d", &s, &e, &c);
            assert(read == 3);
            s--;
            e--;
            d[s][e] += c;
        }

        /*
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < m; j++) {
                if (d[i][j] != 0) {
                    printf("%d->%d: %d\n", i+1, j+1, d[i][j]);
                }
            }
        }
        */

        // int ret;
        fclose(fin);
    }

    void clear() {
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < m; j++) {
                f[i][j] = 0;
            }
        }
    }

    int flow() {
        bool visited[300];
        bzero(visited, sizeof(bool) * m);
        
        int visits[300];
        int froms[300];
        int flows[300];

        int tail = 0;
        int head = 1;
        froms[0] = -1;
        visits[0] = 0;
        visited[0] = true;
        bool reached = false;

        while (head > tail) {
            int visit = visits[tail];

            for (int i = 0; i < m; i++) {
                /* if (visit == i) {
                    continue;
                }
                */

                if (visited[i]) {
                    continue;
                }

                int _d = d[visit][i];
                int _f = f[visit][i];
                if (_f < _d) {
                    visited[i] = true;
                    visits[head] = i;
                    froms[head] = tail;
                    flows[head] = _d - _f;

                    if (i == m - 1) {
                        reached = true;
                        break;
                    }

                    head++;
                }
            }

            if (reached) {
                break;
            }

            tail++;
        }

        if (!reached) {
            return 0;
        }

        int min = 20000000;
        int ptr = head;

        while (ptr > 0) {
            int _f = flows[ptr];
            /*
            printf("> %d -> %d / %d -> %d %d\n", 
                    froms[ptr], ptr,
                    visits[froms[ptr]] + 1,
                    visits[ptr] + 1, flows[ptr]);
            */
            if (_f < min) {
                min = _f;
            }
            ptr = froms[ptr];
        }

        assert(min > 0);
        ptr = head;
        while (ptr > 0) {
            int to = visits[ptr];
            int from = visits[froms[ptr]];
            /*
            printf("! %d -> %d / %d -> %d \n",
                    froms[ptr], ptr,
                    from + 1, to + 1);
            */
            f[from][to] += min;
            f[to][from] -= min;

            assert(f[from][to] <= d[from][to]);
            assert(f[to][from] <= d[to][from]);
            assert(f[from][to] == -f[to][from]);

            ptr = froms[ptr];
        }

        return min;
    }

    void writeOut() {
        clear();
        while (true) {
            int f = flow();
            if (f == 0) {
                break;
            }
        }
        
        int sum = 0;
        for (int i = 0; i < m; i++) {
            sum += f[0][i];
        }

        /*
        for (int i = 0; i < m; i++) {
            int checksum = 0;
            for (int j = 0; j < m; j++) {
                if (d[i][j] > 0 || d[j][i] > 0) {
                    if (f[i][j] < d[i][j]) {
                        printf("%d->%d: %d / (%d, %d)\n",
                                i+1, j+1, f[i][j], -d[j][i], d[i][j]);
                    } else {
                        printf("// %d->%d: %d / (%d, %d)\n",
                                i+1, j+1, f[i][j], -d[j][i], d[i][j]);
                    }
                } else {
                    assert(f[i][j] == 0);
                }
                checksum += f[i][j];
            }
            if (i != 0 && i != m-1) {
                assert(checksum == 0);
            } else {
                // printf("%d %d\n", i, checksum);
            }
        }
        */

        fout = fopen("ditch.out", "w"); assert(fout);
        fprintf(fout, "%d\n", sum);
        fclose(fout);
    }
};

Prob prob;

int main() {
    prob.readIn();
    prob.writeOut();
    return 0;
}

