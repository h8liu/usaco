/*
ID: liulonn1
PROG: vans
LANG: C++
*/

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <stdint.h>
#include <vector>

using std::vector;

static const uint64_t mask = 0xffffffff;

struct BigInt {
    vector<uint64_t> nums;

    int size() const { return nums.size(); }

    void expand(int n) {
        while (nums.size() < n) {
            nums.push_back(0);
        }
    }

    void trim() {
        while (nums.size() > 0 && nums[nums.size()-1] == 0) {
            nums.pop_back();
        }
    }

    void set(uint64_t v) {
        nums.clear();
        if (v == 0) {
            return;
        }
        uint64_t hi = (v >> 32) & mask;
        uint64_t lo = v & mask;
        nums.push_back(lo);
        if (hi > 0) {
            nums.push_back(hi);
        }
    }

    void setZero() {
        nums.clear();
    }

    void add(const BigInt& other) {
        expand(other.size()+1);

        int n = size();
        for (int i = 0; i < n; i++) {
            uint64_t v = nums[i];
            if (i < other.size()) {
                v += other.nums[i];
            }
            nums[i] = v & mask;
            if (i < n) {
                nums[i+1] += (v >> 32) & mask;
            }
        }

        assert(nums[n-1] < 0x100000000);
    }

    void clone(const BigInt& other) {
        nums.assign(other.nums.begin(), other.nums.end());
    }

    int divmod10() {
        int n = size();
        for (int i = n-1; i >= 0; i--) {
            uint64_t v = nums[i];
            nums[i] = v / 10;
            uint64_t mod = v % 10;
            if (i > 0) {
                nums[i-1] += mod * 0x100000000;
            } else {
                return static_cast<int>(mod);
            }
        }
        return 0;
    }

    bool isZero() {
        int n = size();
        if (n == 0) { return true; }
        for (int i = 0; i < size(); i++) {
            if (nums[i] != 0) { return false; }
        }
        return true;
    }

    void fprint(FILE* f) {
        BigInt copy;
        copy.clone(*this);
        copy.trim();
        
        if (copy.isZero()) {
            fprintf(f, "0");
            return;
        }

        vector<int> digits;
        while (!copy.isZero()) {
            int m = copy.divmod10();
            digits.push_back(m);
        }

        vector<int> res(digits.rbegin(), digits.rend());
        for (int i = 0; i < res.size(); i++) {
            fprintf(f, "%d", res[i]);
        }
    }
};

enum conn {
    pnpn_1111,
    pnpn_1122,
    pnpn_1221,

    pnxx,
    pxxn,
    xpnx,
    xxpn,

    NCONN,
};

struct Prob {
    FILE * fin;
    FILE * fout;
    uint64_t n;
    BigInt res;
    
    void readIn() {
        fin = fopen("vans.in", "r"); assert(fin);
        int ret = fscanf(fin, "%llu", &n);
        assert(ret == 1);
        fclose(fin);
    }

    void solve() {
        if (n <= 1) {
            res.setZero();
            return;
        }

        BigInt buf1[NCONN];
        BigInt buf2[NCONN];

        BigInt* cur = buf1;
        BigInt* last = buf2;

        for (int i = 0; i < NCONN; i++) {
            cur[i].setZero();
        }
        cur[pnpn_1122].set(1);
        cur[pxxn].set(1);

        for (int i = 1; i < n-1; i++) {
            // swap cur and other
            BigInt* tmp = last;
            last = cur;
            cur = tmp;
            
            cur[pnpn_1111].clone(last[pnpn_1111]);
            cur[pnpn_1111].add(last[pnxx]);
            cur[pnpn_1111].add(last[pxxn]);
            cur[pnpn_1111].add(last[xxpn]);

            cur[pnpn_1122].clone(last[pnpn_1122]);
            cur[pnpn_1122].add(last[pnxx]);
            cur[pnpn_1122].add(last[xxpn]);

            cur[pnpn_1221].clone(last[pnpn_1221]);
            cur[pnpn_1221].add(last[pxxn]);

            cur[pnxx].clone(last[pxxn]);
            cur[pnxx].add(last[pnpn_1221]);

            cur[pxxn].clone(last[pnpn_1122]);
            cur[pxxn].add(last[xpnx]);
            cur[pxxn].add(last[xxpn]);
            cur[pxxn].add(last[pnxx]);

            cur[xpnx].clone(last[pxxn]);

            cur[xxpn].clone(last[pxxn]);
            cur[xxpn].add(last[pnpn_1221]);
        }
        
        res.clone(cur[pxxn]);
        res.add(cur[pnpn_1221]);

        // Make a double.
        BigInt copy;
        copy.clone(res);
        res.add(copy);
    }

    void writeOut() {
        fout = fopen("vans.out", "w"); assert(fout);
        res.fprint(fout);
        fprintf(fout, "\n");
        fclose(fout);
    }
};

Prob prob;

int main() {
    prob.readIn();
    prob.solve();
    prob.writeOut();
    return 0;
}

