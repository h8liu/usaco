/*
ID: liulonn1
PROG: prime3
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <algorithm>

#include <set>
#include <vector>
#include <map>

using std::map;
using std::set;
using std::vector;

struct Sol {
    vector<int> nums;

    bool operator < (const Sol & other) const {
        for (int i = 0; i < 5; i++) {
            if (nums[i] < other.nums[i]) {
                return true;
            }
            if (nums[i] > other.nums[i]) {
                return false;
            }
        }
        return false;
    }

    void print(FILE * out) {
        for (int i = 0; i < 5; i++) {
            fprintf(out, "%d\n", nums[i]);
        }
    }
};

struct NumSet {
    set<int> s;
};

struct Index {
    map<int, NumSet *> nums;

    void add(int index, int i) {
        map<int, NumSet *>::iterator it = nums.find(index);
        if (it == nums.end()) {
            // not found
            NumSet * nset = new NumSet();
            nset->s.insert(i);
            nums[index] = nset;
        } else {
            it->second->s.insert(i);
        }
    }

    NumSet * list(int index) {
        map<int, NumSet *>::iterator it = nums.find(index);
        if (it == nums.end()) {
            return NULL;
        }
        return it->second;
    }

    bool empty() {
        return nums.empty();
    }
};

struct Prob {
    FILE * fin;
    FILE * fout;
    
    int sum;
    int corner;

    bool notprime[99999];

    set<int> primes;
    Index endPrimes; // indexed by 00001
    Index borderPrimes; // indexed by 10001, no zeros;
    Index d1Primes; // indexed by 10001
    Index d2Primes; // indexed by 10101
    Index p2Primes; // indexed by 11011

    vector<Sol> sols;
    
    void readIn() {
        fin = fopen("prime3.in", "r"); assert(fin);
        int ret = fscanf(fin, "%d %d", &sum, &corner);
        assert(ret == 2);
        assert(corner > 0 && corner <= 9);

        fclose(fin);
    }

    bool all1379(int i) {
        while (i > 0) {
            int d = i % 10;
            i /= 10;
            if (d % 2 == 0 || d == 5) {
                return false;
            }
        }
        return true;
    }

    bool sumMatch(int i) {
        int s = 0;
        while (i > 0) {
            s += i % 10;
            i /= 10;
        }
        return s == sum;
    }

    bool noZero(int i) {
        while (i > 0) {
            int d = i % 10;
            i /= 10;
            if (d == 0) {
                return false;
            }
        }
        return true;
    }

    inline int n1(int i) { return i / 10000; }
    inline int n2(int i) { return i / 1000 % 10; }
    inline int n3(int i) { return i / 100 % 10; }
    inline int n4(int i) { return i / 10 % 10; }
    inline int n5(int i) { return i % 10; }
    inline int d2Index(int i) { return n1(i) * 100 + n3(i) * 10 + n5(i); }
    inline int p2Index(int i) { return i - n3(i) * 100; }

    void writeOut() {
        bzero(notprime, sizeof(bool) * 99999);

        for (int i = 2; i < 99999; i++) {
            if (notprime[i] == false) {
                if (i >= 10000 && sumMatch(i)) {
                    primes.insert(i);

                    if (all1379(i)) {
                        endPrimes.add(n5(i), i);
                    }
                    if (n1(i) == corner && noZero(i)) {
                        borderPrimes.add(n5(i), i);
                    }
                    if (n1(i) == corner) {
                        d1Primes.add(n5(i), i);
                    }
                    d2Primes.add(d2Index(i), i);
                    p2Primes.add(p2Index(i), i);
                }

                for (int j = i * 2; j < 99999; j+=i) {
                    notprime[j] = true;
                }
            }
        }

        fout = fopen("prime3.out", "w"); assert(fout);

        if (sum % 3 == 0 || endPrimes.empty() || borderPrimes.empty() ||
                d1Primes.empty() || d2Primes.empty()
                ) {
            fprintf(fout, "NONE\n");
        } else {
            fill();

            if (sols.empty()) {
                fprintf(fout, "NONE\n");
            } else {
                std::sort(sols.begin(), sols.end());

                for (unsigned int i = 0; i < sols.size(); i++) {
                    if (i > 0) {
                        fprintf(fout, "\n");
                    }

                    sols[i].print(fout);
                }
            }
        }

        fclose(fout);
    }


    int end;
    int right, bottom;
    int top, left;
    int d1, d2;
    int row2, row4;
    int col2, col4;
    
    void fill() {
        end = 1;
        fillEnd();
        end = 3;
        fillEnd();
        end = 7;
        fillEnd();
        end = 9;
        fillEnd();
    }

    void fillEnd() {
        NumSet * s = endPrimes.list(end);
        if (s == NULL) { return; }
        for (set<int>::iterator it = s->s.begin();
                it != s->s.end(); it++) {
            right = *it;
            
            for (set<int>::iterator it2 = s->s.begin();
                    it2 != s->s.end(); it2++) {
                bottom = *it2;
                fillBorder();
            }
        }
    }

    void fillBorder() {
        NumSet * tops = borderPrimes.list(n1(right)); // for top
        if (tops == NULL) { return; }

        for (set<int>::iterator it = tops->s.begin();
                it != tops->s.end(); it++) {
            top = *it;
            NumSet * lefts = borderPrimes.list(n1(bottom));
            if (lefts == NULL) { continue; }
            for (set<int>::iterator it2 = lefts->s.begin();
                    it2 != lefts->s.end(); it2++) {
                left = *it2;
                fillDiag();
            }
        }
    }

    void fillDiag() {
        NumSet * d1s = d1Primes.list(end);
        if (d1s == NULL) { return; }
        for (set<int>::iterator it = d1s->s.begin();
                it != d1s->s.end(); it++) {
            d1 = *it;
            NumSet * d2s = d2Primes.list(n5(left) * 100 + n3(d1) * 10 + n1(right));
            if (d2s == NULL) { continue; }
            for (set<int>::iterator it2 = d2s->s.begin();
                    it2 != d2s->s.end(); it2++) {
                d2 = *it2;
                fillRows();
            }
        }
    }

    void fillRows() {
        int r2Index = n2(left) * 10000 + n2(d1) * 1000 + n4(d2) * 10 + n2(right);
        NumSet * r2s = p2Primes.list(r2Index);
        if (r2s == NULL) { return; }
        for (set<int>::iterator it1 = r2s->s.begin();
                it1 != r2s->s.end(); it1++) {
            row2 = *it1;

            int r4Index = n4(left) * 10000 + n2(d2) * 1000 + n4(d1) * 10 + n4(right);
            NumSet * r4s = p2Primes.list(r4Index);
            if (r4s == NULL) { continue; }
            for (set<int>::iterator it2 = r4s->s.begin();
                    it2 != r4s->s.end(); it2++) {
                row4 = *it2;
                fillCols();
            }
        }
    }

    void fillCols() {
        int c2Index = n2(top) * 10000 + n2(d1) * 1000 + n2(d2) * 10 + n2(bottom);
        NumSet * c2s = p2Primes.list(c2Index);
        if (c2s == NULL) { return; }
        for (set<int>::iterator it1 = c2s->s.begin();
                it1 != c2s->s.end(); it1++) {
            col2 = *it1;

            int c4Index = n4(top) * 10000 + n4(d2) * 1000 + n4(d1) * 10 + n4(bottom);
            NumSet * c4s = p2Primes.list(c4Index);
            if (c4s == NULL) { continue; }
            for (set<int>::iterator it2 = c4s->s.begin();
                    it2 != c4s->s.end(); it2++) {
                col4 = *it2;
                finalCheck();
            }
        }
    }

    void finalCheck() {
        int row3 = n3(left) * 10000 + n3(col2) * 1000 + n3(d1) * 100 + 
            n3(col4) * 10 + n3(right);
        if (primes.find(row3) == primes.end()) {
            return;
        }
        int col3 = n3(top) * 10000 + n3(row2) * 1000 + n3(d1) * 100 +
            n3(row4) * 10 + n3(bottom);
        if (primes.find(col3) == primes.end()) {
            return;
        }
        
        sols.push_back(Sol());
        Sol & s = *sols.rbegin();
        s.nums.push_back(top);
        s.nums.push_back(row2);
        s.nums.push_back(row3);
        s.nums.push_back(row4);
        s.nums.push_back(bottom);
    }

};

Prob prob;

int main() {
    prob.readIn();
    prob.writeOut();
    return 0;
}

