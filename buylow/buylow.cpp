/*
ID: liulonn1
PROG: buylow
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>

#include <map>
#include <set>
#include <vector>
#include <deque>

using std::map;
using std::set;
using std::vector;
using std::deque;

#define NumBase 1000000

struct Num {
    vector<int> d;

    void clear() { d.clear(); }

    bool empty() { return d.empty(); }

    void one() {
        d.clear();
        d.push_back(1);
    }

    void add(Num & other) {
        int carry = 0;
        unsigned int i;
        for (i = 0; i < other.d.size(); i++) {
            if (i >= d.size()) {
                d.push_back(0);
            }
            int sum = other.d[i] + d[i] + carry;
            d[i] = sum % NumBase;
            carry = sum / NumBase;
        }

        while (carry > 0) {
            if (i >= d.size()) {
                d.push_back(carry);
                break;
            } else {
                int sum = d[i] + carry;
                d[i] = sum % NumBase;
                carry = sum / NumBase;
            }
        }
    }

    void print(FILE * out) {
        if (d.empty()) {
            fprintf(out, "0");
        }

        for (int i = int(d.size()) - 1; i >= 0; i--) {
            if (i == int(d.size()) - 1) {
                fprintf(out, "%d", d[i]);
            } else {
                fprintf(out, "%06d", d[i]);
            }
        }
    }
};

struct Data {
    int price;
    int combo;
    vector<int> lasts;
    vector<int> nexts;
    bool reachable;
    bool reached;
};

struct Suffix {
    int id;
    int price;
    int atom;
    Num count;
};

struct SufGroup {
    set<int> members; // suffixes

    vector<int> atoms; // atom suffix groups
    int atomId;
    int sufId; 

    void addMember(int sufId) {
        members.insert(sufId);
    }

    bool haveMember(int sufId) {
        return members.find(sufId) != members.end();
    }

    bool empty() { return members.empty(); }

    SufGroup * intersect(SufGroup *g) {
        SufGroup * ret = new SufGroup();

        for (set<int>::iterator it = members.begin();
                it != members.end(); it++) {
            if (g->members.find(*it) == g->members.end()) {
                continue;
            }
            ret->addMember(*it);
        }

        if (ret->empty()) {
            delete ret;
            return NULL;
        }
        return ret;
    }

    void sub(SufGroup * g) {
        for (set<int>::iterator it = g->members.begin();
                it != g->members.end(); it++) {
            members.erase(*it);
        }
    }

    SufGroup * clone() {
        SufGroup * ret = new SufGroup();

        for (set<int>::iterator it = members.begin();
                it != members.end(); it++) {
            ret->addMember(*it);
        }

        return ret;
    }

    int size() {
        return members.size();
    }
};

struct AtomBuilder {
    vector<SufGroup *> atoms;

    void add(SufGroup *g) {
        g = g->clone();
        vector<SufGroup *> buf;
        for (vector<SufGroup *>::iterator it = atoms.begin();
                it != atoms.end(); it++) {
            SufGroup *inter = (*it)->intersect(g);
            if (inter == NULL) {
                continue;
            }

            assert(!inter->empty());

            if (inter->size() < (*it)->size()) {
                (*it)->sub(inter);
                buf.push_back(inter);
            }

            g->sub(inter);
        }

        if (g->empty()) {
            delete g;
        } else {
            buf.push_back(g);
        }

        for (vector<SufGroup *>::iterator it = buf.begin();
                it != buf.end(); it++) {
            assert(!(*it)->empty());
            atoms.push_back(*it);
        }
    }
};

// NodeGroup are last nodes that has the same prices
struct NodeGroup {
    int price;

    map<int, SufGroup *> nodes; // index by position

    void buildAtoms(AtomBuilder *builder) {
        for (map<int, SufGroup *>::iterator it = nodes.begin();
                it != nodes.end(); it++) {
            SufGroup *sg = it->second;
            builder->add(sg);
        }
    }

    void markAtoms(AtomBuilder *builder) {
        for (map<int, SufGroup *>::iterator it = nodes.begin();
                it != nodes.end(); it++) {
            for (vector<SufGroup *>::iterator jt = builder->atoms.begin();
                    jt != builder->atoms.end(); jt++) {
                SufGroup *g = it->second;
                SufGroup *atom = *jt;
                assert(!atom->empty());
                if (g->haveMember(*atom->members.begin())) {
                    g->atoms.push_back(atom->atomId);
                }
            }
        }
    }
};

struct Counter {
    vector<Suffix *> sufs;
    vector<SufGroup *> atoms;
    map<int, NodeGroup*> nodegs; // map from price to index to suffix groups

    Suffix * newSuf(int price) {
        Suffix * ret = new Suffix();
        ret->id = sufs.size();
        ret->price = price;

        sufs.push_back(ret);
        return ret;
    }

    void  addEndSuf() {
        Suffix * ret = newSuf(0);
        ret->atom = -1;
    }

    void bindNode(int price, int index, int suf) {
        map<int, NodeGroup*>::iterator it = nodegs.find(price);
        if (it == nodegs.end()) {
            // not found
            NodeGroup *g = new NodeGroup();
            g->price = price;
            SufGroup *sg = new SufGroup();
            sg->addMember(suf);

            g->nodes[index] = sg;

            nodegs[price] = g;
        } else {
            NodeGroup *g = it->second;
            assert(g->price == price);

            map<int, SufGroup*>::iterator found = g->nodes.find(index);
            if (found == g->nodes.end()) {
                // not found
                SufGroup *sg = new SufGroup();
                sg->addMember(suf);
                g->nodes[index] = sg;
            } else {
                g->nodes[index]->addMember(suf);
            }
        }
    }

    void buildAtoms() {
        for (map<int, NodeGroup *>::iterator it = nodegs.begin();
                it != nodegs.end(); it++) {
            int price = it->first;
            NodeGroup * ng = it->second;
            AtomBuilder builder;
            ng->buildAtoms(&builder);
            
            // register atoms
            for (vector<SufGroup *>::iterator it = builder.atoms.begin();
                    it != builder.atoms.end(); it++) {
                SufGroup * sg = *it; // the atom
                sg->atomId = atoms.size();
                atoms.push_back(sg);

                // for each atom, create a suf
                Suffix * suf = newSuf(price);
                suf->atom = sg->atomId;
                sg->sufId = suf->id;
            }

            ng->markAtoms(&builder);
        }
    }

    void bindNextLevel(Data * data) {
        vector<NodeGroup *> old;
        for (map<int, NodeGroup *>::iterator it = nodegs.begin();
                it != nodegs.end(); it++) {
            old.push_back(it->second);
        }

        nodegs.clear();
        for (vector<NodeGroup *>::iterator it = old.begin();
                it != old.end(); it++) {
            NodeGroup * ng = *it;
        
            for (map<int, SufGroup *>::iterator jt = ng->nodes.begin();
                    jt != ng->nodes.end(); jt++) {
                int index = jt->first;
                Data & d = data[index];
                SufGroup * sg = jt->second;

                for (vector<int>::iterator at = sg->atoms.begin();
                        at != sg->atoms.end(); at++) {
                    int sufId = atoms[*at]->sufId;

                    for (vector<int>::iterator nt = d.lasts.begin();
                            nt != d.lasts.end(); nt++) {
                        bindNode(data[*nt].price, *nt, sufId);
                    }
                }

                delete sg;
            }

            delete ng;
        }
    }

    void count(Num & total) {
        total.clear();

        for (vector<Suffix *>::reverse_iterator it = sufs.rbegin();
                it != sufs.rend(); it++) {
            Suffix *s = *it;

            if (s->count.empty()) {
                s->count.one();
            }

            if (s->atom < 0) {
                total.add(s->count);
                continue;
            }

            SufGroup * atom = atoms[s->atom];
            for (set<int>::iterator it = atom->members.begin();
                    it != atom->members.end(); it++) {
                Suffix *next = sufs[*it];
                next->count.add(s->count);
            }
        }
    }

    void print() {
        for (vector<Suffix *>::iterator it = sufs.begin(); 
                it != sufs.end(); it++) {
            Suffix *s = *it;
            
            fprintf(stderr, "#%d %d: ", s->id, s->price);
            if (s->atom < 0) {
                fprintf(stderr, "<end> -> ");
                s->count.print(stderr);
                fprintf(stderr, "\n");
                continue;
            }

            SufGroup * atom = atoms[s->atom];
            for (set<int>::iterator it = atom->members.begin();
                    it != atom->members.end(); it++) {
                fprintf(stderr, "%d ", *it);
            }

            fprintf(stderr, " -> ");
            s->count.print(stderr);
            fprintf(stderr, "\n");
        }
    }
};

struct Prob {
    FILE * fin;
    FILE * fout;

    int n;
    Data data[5000];
    
    void readIn() {
        fin = fopen("buylow.in", "r"); assert(fin);
        int ret = fscanf(fin, "%d", &n);
        assert(ret == 1);

        for (int i = 0; i < n; i++) {
            ret = fscanf(fin, "%d", &(data[i].price));
            assert(ret == 1);
        }

        fclose(fin);
    }

    int findLongestCombo() {
        for (int i = 0; i < n; i++) {
            // find the longest combo
            int combo = 0; // the longest combo
            for (int j = 0; j < i; j++) {
                if (data[i].price < data[j].price) {
                    int c = data[j].combo;
                    // printf("i=%d, j=%d, c=%d\n", i, j, c);
                    if (c > combo) {
                        combo = c;
                    }
                }
            }

            if (combo > 0) {
                for (int j = 0; j < i; j++) {
                    if (data[i].price < data[j].price 
                            && data[j].combo == combo) {
                        data[i].lasts.push_back(j);
                        data[j].nexts.push_back(i);
                    }
                }
            }

            data[i].combo = combo + 1;
        }

        int maxCombo = 0;
        for (int i = 0; i < n; i++) {
            int c = data[i].combo;
            if (c > maxCombo) {
                maxCombo = c;
            }
        }
        
        return maxCombo;
    }

    void markReachable(int maxCombo) {
        deque<int> queue;

        for (int i = 0; i < n; i++) {
            Data & d = data[i];
            if (d.combo == maxCombo) {
                d.reachable = true;
                queue.push_back(i);
            } else {
                d.reachable = false;
            }
        }

        while (!queue.empty()) {
            int i = queue.front();
            queue.pop_front();
            Data & di = data[i];
            
            for (vector<int>::iterator it = di.lasts.begin();
                    it != di.lasts.end(); it++) {
                int j = *it;
                Data & dj = data[j];
                if (!dj.reachable) {
                    dj.reachable = true;
                    queue.push_back(j);
                }
            }
        }
    }

    void count(int maxCombo, Num & ret) {
        Counter counter;

        counter.addEndSuf();

        for (int i = 0; i < n; i++) {
            Data & di = data[i];
            if (di.combo != maxCombo) { continue; }
            counter.bindNode(di.price, i, 0);
        }

        for (int i = 0; i < maxCombo; i++) {
            // for the same price node, build the suf groups
            counter.buildAtoms();
            counter.bindNextLevel(data);
        }

        counter.count(ret);
        // counter.print();
    }

    void writeOut() {
        int maxCombo = findLongestCombo();
        // fprintf(stderr, "maxCombo=%d\n", maxCombo);

        // markReachable(maxCombo);
        Num total;
        count(maxCombo, total);

        fout = fopen("buylow.out", "w"); assert(fout);
        fprintf(fout, "%d ", maxCombo);
        total.print(fout);
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

