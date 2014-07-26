/*
ID: liulonn1
PROG: heritage
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>

struct Prob {
    char path[50];
    char name[50];
    FILE * fin;
    FILE * fout;
    void setName(const char * s) { strcpy(name, s); }


    char seq1[50];
    char seq2[50];

    void readIn() {
        sprintf(path, "%s.in", name);
        fin = fopen(path, "r");
        assert(fin);
        int ret;
        
        ret = fscanf(fin, "%s", seq1); assert(ret == 1);
        ret = fscanf(fin, "%s", seq2); assert(ret == 1);
        assert(strlen(seq1) == strlen(seq2));
        fclose(fin);
    }

    void trav(char * s1, char * s2, char ** outs1, char ** outs2, char ending) {
        char parent = s2[0];
        char * p1 = s1;
        char * p2 = s2;
        p2++;

        // printf("%c %c\n", parent, ending);

        if (*p1 != parent) {
            trav(p1, p2, &p1, &p2, parent);
        }

        assert(*p1 == parent);
        p1++;
        
        if (*p1 != ending) {
            trav(p1, p2, &p1, &p2, ending);
        }

        assert(*p1 == ending);
        
        fprintf(fout, "%c", parent);

        *outs1 = p1;
        *outs2 = p2;
    }

    void writeOut() {
        sprintf(path, "%s.out", name);
        fout = fopen(path, "w");
        
        char * s1 = seq1;
        char * s2 = seq2;
        trav(s1, s2, &s1, &s2, '\0');
        fprintf(fout, "\n");

        fclose(fout);
    }
};

int main() {
    Prob prob;
    prob.setName("heritage");
    prob.readIn();
    prob.writeOut();

    return 0;
}

