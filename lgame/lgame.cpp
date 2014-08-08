/*
ID: liulonn1
PROG: lgame
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>

#include <map>
#include <vector>
using std::map;
using std::vector;

struct CharScore { char c, score; };

CharScore scores[26] = {
    { 'q', 7 },
    { 'w', 6 },
    { 'e', 1 },
    { 'r', 2 },
    { 't', 2 },
    { 'y', 5 },
    { 'u', 4 },
    { 'i', 1 },
    { 'o', 3 },
    { 'p', 5 },
    { 'a', 2 },
    { 's', 1 },
    { 'd', 4 },
    { 'f', 6 },
    { 'g', 5 },
    { 'h', 5 },
    { 'j', 7 },
    { 'k', 6 },
    { 'l', 3 },
    { 'z', 7 },
    { 'x', 7 },
    { 'c', 4 },
    { 'v', 6 },
    { 'b', 5 },
    { 'n', 2 },
    { 'm', 5 },
};

map<char, char> scoreMap;

struct Word {
    char word[8];
    map<char, char> count;
    map<char, char> leftover;
    char len;
    char score;
    int id;

    char calcScore() {
        char ret = 0;
        for (map<char, char>::iterator it = count.begin();
                it != count.end(); it++) {
            ret += scoreMap[it->first] * it->second;
        }
        return ret;
    }

    bool use(const map<char, char> & usable, map<char, char> *leftover=NULL) {

        for (map<char, char>::iterator it = count.begin();
                it != count.end(); it++) {
            char c = it->first;
            map<char, char>::const_iterator quota = usable.find(c);
            if (quota == usable.end()) {
                return false;
            }
            if (quota->second < it->second) {
                return false;
            }
        }

        if (leftover != NULL) {
            for (map<char, char>::const_iterator quota = usable.begin();
                    quota != usable.end(); quota++) {
                char c = quota->first;
                map<char, char>::const_iterator used = count.find(c);
                int nused;
                if (used == count.end()) {
                    nused = 0;
                } else {
                    nused = used->second;
                }

                if (nused < quota->second) {
                    (*leftover)[c] = quota->second - nused;
                }
            }
        }

        return true;
    }
};

struct Sol {
    Word *w1;
    Word *w2;

    char score() {
        if (w2 != NULL) {
            return w1->score + w2->score;
        }
        return w1->score;
    }

    void print(FILE *out) {
        if (w2 != NULL) {
            fprintf(out, "%s %s\n", w1->word, w2->word);
        } else {
            fprintf(out, "%s\n", w1->word);
        }
    }

    void dprint(FILE *out) {
        int s = score();
        if (w2 != NULL) {
            fprintf(out, "  %s %s = %d\n", w1->word, w2->word, s);
        } else {
            fprintf(out, "%s = %d\n", w1->word, s);
        }
    }
};

struct Prob {
    FILE * fin;
    FILE * fout;
    Word *target;
    vector<Word *> dict;
    vector<Word *> dict3;
    vector<Word *> dict4;
    vector<Word *> dict34;

    vector<Sol> sols;

    void buildScoreMap() {
        for (int i = 0; i < 26; i++) {
            scoreMap[scores[i].c] = scores[i].score;
        }
    }

    Word * makeWord(char *w) {
        Word * ret = new Word();
        memcpy(ret->word, w, sizeof(char) * 8);
        
        int i;
        for (i = 0; i < 7; i++) {
            char c = w[i];
            if (c == '\0') break;
            map<char, char>::iterator it = ret->count.find(c);
            if (it == ret->count.end()) {
                ret->count[c] = 1;
            } else {
                ret->count[c]++;
            }
        }
        
        ret->len = i;
        ret->score = ret->calcScore();

        return ret;
    }
    
    void readIn() {
        buildScoreMap();

        char buf[10];
        fin = fopen("lgame.in", "r"); assert(fin);
        int ret = fscanf(fin, "%s", buf); assert(ret == 1);
        fclose(fin);

        target = makeWord(buf);

        fin = fopen("lgame.dict", "r"); assert(fin);
        while (true) {
            ret = fscanf(fin, "%s", buf); assert(ret);
            if (buf[0] == '.') break;
            Word * w = makeWord(buf);

            if (w->use(target->count, &(w->leftover))) {
                w->id = dict.size();
                dict.push_back(w);
                if (w->len == 3) dict3.push_back(w);
                if (w->len == 4 || w->len == 3) dict34.push_back(w);
            } else {
                delete w;
            }
        }

        fclose(fin);
    }

    int maxScore;

    void record(Sol & sol) {
        // sol.dprint(stdout);
        int s = sol.score();
        if (s > maxScore) {
            maxScore = s;
            sols.clear();
            sols.push_back(sol);
        } else if (s == maxScore) {
            sols.push_back(sol);
        }
    }

    void writeOut() {
        maxScore = 0;
        
        Sol sol;
        for (vector<Word *>::iterator it = dict.begin();
                it != dict.end(); it++) {
            Word * w = *it;
            // printf("%s (id=%d len=%d)\n", w->word, w->id, w->len);
            /*
            for (map<char, char>::iterator q = w->leftover.begin();
                    q != w->leftover.end(); q++) {
                printf("  -- %c %d\n", q->first, q->second);
            }
            */

            sol.w1 = w;
            sol.w2 = NULL;
            record(sol);

            if (w->len == 3) {
                for (vector<Word *>::iterator it = dict34.begin();
                        it != dict34.end(); it++) {
                    Word * w2 = *it;
                    if (w2->id <= w->id) { continue; }
                    // printf("  %s (id=%d len=%d)\n", w2->word, w2->id, w2->len);

                    if (!w2->use(w->leftover)) { continue; }
                    sol.w2 = w2;
                    record(sol);
                }
            } else if (w->len == 4) {
                for (vector<Word *>::iterator it = dict3.begin();
                        it != dict3.end(); it++) {
                    Word * w2 = *it;
                    if (w2->id <= w->id) { continue; }
                    // printf("  %s (id=%d len=%d)\n", w2->word, w2->id, w2->len);

                    if (!w2->use(w->leftover)) { continue; }
                    sol.w2 = w2;
                    record(sol);
                }
            }
        }

        fout = fopen("lgame.out", "w"); assert(fout);

        fprintf(fout, "%d\n", maxScore);

        for (vector<Sol>::iterator it = sols.begin(); it != sols.end(); it++) {
            it->print(fout);
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

