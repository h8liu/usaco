/*
ID: liulonn1
PROG: cryptcow
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <map>

const char * TARGET = "Begin the Escape execution at the Break of Dawn";
int TARGET_LEN;
std::map<int, int> calc;

inline int concat(int a, int b) {
    std::map<int, int>::iterator it = calc.find(a * 4096 + b);
    if (it == calc.end()) {
        return -1;
    }
    return it->second;
}

struct Part {
    char c;
    int begins[100];
    int nbegin;
    int len;
    int id;

    int code() {
        if (c < 3) { return c; }
        assert(nbegin > 0);
        return begins[0] * 128 + len + 3;
    }

    void print() {
        if (c == 'C' || c == 'O' || c == 'W') {
            printf("[%c]", c);
            return;
        }

        assert(nbegin > 0);
        int begin = begins[0];;
        int end = begin + len;
        for (int i = begin; i < end; i++) {
            printf("%c", TARGET[i]);
        }
    }

    void printSubs() {
        char buf[100];
        for (int i = 0; i < nbegin; i++) {
            for (int j = begins[i]; j < begins[i] + len; j++) {
                buf[j - begins[i]] = TARGET[j];
            }
            buf[len] = '\0';
            printf("  sub %d: %d %d: %s\n", i, begins[i], len,
                    buf);
        }
    }

    int fillWith(char * buf) {
        for (int i = 0; ; i++) {
            char c = buf[i];
            if (c == 'C' || c == 'O' || c == 'W' || c == '\0') {
                if (i == 0) {
                    this->c = c;
                    this->nbegin = 0;
                    return 1;
                }

                this->c = ' ';
                this->nbegin = 0;

                for (int j = 0; j < TARGET_LEN; j++) {
                    bool matched = true;
                    for (int k = 0; k < i; k++) {
                        if (TARGET[j + k] != buf[k]) {
                            matched = false;
                            break;
                        }
                    }

                    if (matched) {
                        begins[nbegin] = j;
                        nbegin++;
                    }
                }
                
                len = i;
                return i;
            }
        }
    }

    bool merge(Part * p1, Part * p2) {
        nbegin = 0;
        c = ' ';
        len = p1->len + p2->len;

        for (int i = 0; i < p1->nbegin; i++) {
            for (int j = 0; j < p2->nbegin; j++) {
                if (p1->begins[i] + p1->len == p2->begins[j]) {
                    begins[nbegin] = p1->begins[i];
                    nbegin++;
                    break;
                }
            }
        }

        return nbegin > 0;
    }

    void clone(Part * other) {
        c = other->c;
        nbegin = other->nbegin;
        len = other->len;
        for (int i = 0; i < nbegin; i++) {
            begins[i] = other->begins[i];
        }
    }
};

struct String {
    int codes[100];
    int npart;

    void addPart(int c) {
        codes[npart] = c;
        npart++;
    }

    bool cat(int a) {
        if (npart > 0 && a >= 3 && codes[npart-1] >= 3) {
            int c = concat(codes[npart-1], a);
            if (c > 0) {
                codes[npart-1] = c;
                return true;
            } else {
                return false;
            }
        }

        codes[npart] = a;
        npart++;
        return true;
    }

    bool swapFrom(String & s, int pc, int po, int pw) {
        npart = 0;
        
        for (int i = 0; i < pc; i++) {
            if (!cat(s.codes[i])) return false;
        }
        for (int i = po + 1; i < pw; i++) {
            if (!cat(s.codes[i])) return false;
        }
        for (int i = pc + 1; i < po; i++) {
            if (!cat(s.codes[i])) return false;
        }
        for (int i = pw + 1; i < s.npart; i++) {
            if (!cat(s.codes[i])) return false;
        }

        return true;
    }
};

struct Prob {
    FILE * fin;
    FILE * fout;
    char buf[100];

    String stack[10];
    
    void readIn() {
        fin = fopen("cryptcow.in", "r"); assert(fin);
        if (fgets(buf, 100, fin) == NULL) {
            exit(-1);
        }

        int n = strlen(buf);
        if (buf[n-1] == '\n') {
            buf[n-1] = '\0';
        }
        fclose(fin);

        TARGET_LEN = strlen(TARGET);
    }

    int checkLevel(int i) {
        String & s = stack[i];
        /*
        printf("%d: ", i);
        s.print();
        printf("\n");
        */
        
        if (s.npart == 1) {
            Part * p = &parts[s.codes[0]];
            if (p->len == TARGET_LEN) {
                return 0;
            }
        }

        if (s.npart <= 3) {
            return -1;
        }

        if (s.codes[0] >= 3 && s.codes[1] != 0) {
            return -1;
        }
        if (s.codes[0] == 1 || s.codes[0] == 2) {
            return -1;
        }
        if (s.codes[s.npart-1] >= 3 
                && s.codes[s.npart-2] != 2) {
            return -1;
        }
        if (s.codes[s.npart-1] == 0 || s.codes[s.npart-1] == 1) {
            return -1;
        }

        for (int posc = 0; posc < s.npart; posc++) {
            if (s.codes[posc] != 0) {
                continue;
            }

            for (int poso = posc+1; poso < s.npart; poso++) {
                if (s.codes[poso] != 1) {
                    continue;
                }

                for (int posw = poso+1; posw < s.npart; posw++) {
                    if (s.codes[posw] != 2) {
                        continue;
                    }

                    assert(i+1 < 10);
                    if (stack[i+1].swapFrom(stack[i], posc, poso, posw)) {
                        int res = checkLevel(i+1);
                        if (res >= 0) {
                            return res + 1;
                        }
                    }
                }
            }
        }

        return -1;
    }

    Part parts[2500];
    std::map<int, Part *> partMap;
    int npart;

    int addPart(Part *p) {
        if (p->c == 'C') return 0;
        if (p->c == 'O') return 1;
        if (p->c == 'W') return 2;

        int c = p->code();
        std::map<int, Part *>::iterator it = partMap.find(c);
        if (it != partMap.end()) {
            return it->second->id;
        }
        assert(p->c == ' ');

        partMap[c] = &parts[npart];
        parts[npart].clone(p);
        parts[npart].id = npart;
        int ret = npart;
        npart++;
        return ret;
    }

    void concatParts() {
        int i = 4;
        while (i < npart) {
            Part *p = &parts[i];
            Part p2;
            for (int j = 3; j < i; j++) {
                if (p2.merge(&parts[j], p)) {
                    int r = addPart(&p2);
                    int a = parts[j].id;
                    int b = p->id;
                    calc[a * 4096 + b] = r;
                }
                if (p2.merge(p, &parts[j])) {
                    int r = addPart(&p2);
                    int a = p->id;
                    int b = parts[j].id;
                    calc[a * 4096 + b] = r;
                }
            }

            i++;
        }
    }

    bool parse(String * s, char * buf) {
        char * pt = buf;

        while (true) {
            int n = parts[npart].fillWith(pt);
            if (parts[npart].c == ' ') {
                if (parts[npart].nbegin == 0) {
                    return false;
                } else {
                    int code = parts[npart].code();
                    std::map<int, Part*>::iterator it 
                        = partMap.find(code);
                    Part * p;
                    if (it == partMap.end()) {
                        parts[npart].id = npart;
                        partMap[code] = &parts[npart];
                        p = &parts[npart];
                        // p->print(); printf("\n");
                        npart++;
                    } else {
                        p = it->second;
                    }
                    // p->print(); printf("\n");
                    s->addPart(p->id);
                }
            } else {
                if (parts[npart].c == '\0') {
                    break;
                }

                char c = parts[npart].c;
                // printf("%c\n", c);
                if (c == 'C') { s->addPart(0); }
                if (c == 'O') { s->addPart(1); }
                if (c == 'W') { s->addPart(2); }
            }
            pt += n;
        }

        return true;
    }

    void writeOut() {
        int ret = -1;
        parts[0].c = 'C'; parts[0].id = 0;
        parts[0].nbegin = 0; parts[0].len = 0;

        parts[1].c = 'C'; parts[1].id = 1;
        parts[1].nbegin = 0; parts[1].len = 0;

        parts[2].c = 'C'; parts[2].id = 2;
        parts[2].nbegin = 0; parts[2].len = 0;
        npart = 3;

        if (parse(&stack[0], buf)) {
            concatParts();
            ret = checkLevel(0);
        }

        fout = fopen("cryptcow.out", "w"); assert(fout);

        if (ret < 0) {
            fprintf(fout, "0 0\n");
        } else {
            fprintf(fout, "1 %d\n", ret);
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

