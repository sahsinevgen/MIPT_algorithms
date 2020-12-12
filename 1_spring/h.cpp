#include<bits/stdc++.h>

using namespace std;

int64_t64_t deg[1000002];
int64_t64_t P = 29;
int64_t64_t mod = 1e9 + 3;


struct Tree{
    int64_t x;
    int64_t y;
    Tree* L = nullptr;
    Tree* R = nullptr;
    int64_t size;
    int64_t64_t hash;
    int64_t64_t rev_hash;
    bool reversed;
    Tree(int64_t64_t x_) {
        x = x_;
        L = nullptr;
        R = nullptr;
        y = rand();
        size = 1;
        reversed = false;
        hash = x_;
        rev_hash = x_;
    }
};

int64_t Size(Tree* t) {
    if (t == nullptr) {
        return 0;
    }
    return t->size;
}

void push(Tree* t){
    if ((t == nullptr) || (!(t->reversed))) {
        return;
    }
    t->reversed = false;
    swap(t->hash, t->rev_hash);
    swap(t->L, t->R);
    if (t->L != nullptr) {
        t->L->reversed = !(t->L->reversed);
    }
    if (t->R != nullptr) {
        t->R->reversed = !(t->R->reversed);
    }
}

void update(Tree* t) {
    if (t == nullptr) {
        return;
    }
    push(t);
    push(t->L);
    push(t->R);
    t->hash = (t->x) * deg[Size(t->L)];
    t->rev_hash = (t->x) * deg[Size(t->R)];
    if (t->L != nullptr) {
        t->hash += t->L->hash;
        t->rev_hash += (t->L->rev_hash) * deg[Size(t->R) + 1] % mod;
    }
    if (t->R != nullptr) {
        t->hash += (t->R->hash) * deg[Size(t->L) + 1] % mod;
        t->rev_hash += t->R->rev_hash;
    }
    t->hash %= mod;
    t->rev_hash %= mod;
    t->size = Size(t->L) + Size(t->R) + 1;
}

pair<Tree*, Tree*> split(Tree* t, int64_t k) {
    if (t == nullptr) {
        return {nullptr, nullptr};
    }
    push(t);
    int64_t sz = Size(t->L);
    if (sz <= k) {
        pair<Tree*, Tree*> tt = split(t->R, k - sz - 1);
        t->R = tt.first;
        update(t);
        return {t, tt.second};
    }
    else {
        pair<Tree*, Tree*> tt = split(t->L, k);
        t->L = tt.second;
        update(t);
        return {tt.first, t};
    }
}

Tree* merge(Tree* a, Tree* b) {
    if (a == nullptr) {
        return b;
    }
    if (b == nullptr) {
        return a;
    }
    push(a);
    push(b);
    if (a->y > b->y) {
        a->R = merge(a->R, b);
        update(a);
        return a;
    }
    else {
        b->L = merge(a, b->L);
        update(b);
        return b;
    }
}

void rev(Tree* t, int64_t l, int64_t r) {
    pair<Tree*, Tree*> tt1 = split(t, r);
    pair<Tree*, Tree*> tt2 = split(tt1.first, l - 1);
    tt2.second->reversed = !(tt2.second->reversed);
    Tree* t2 = merge(tt2.first, tt2.second);
    t = merge(t2, tt1.second);
}

int64_t64_t get_hash(Tree* t, int64_t l, int64_t r) {
    pair<Tree*, Tree*> tt1 = split(t, r);
    pair<Tree*, Tree*> tt2 = split(tt1.first, l - 1);
    int64_t64_t hash = tt2.second->hash;
    Tree* t2 = merge(tt2.first, tt2.second);
    t = merge(t2, tt1.second);
    return hash;
}

int64_t get_ans(Tree* t, int64_t l, int64_t r) {
    int64_t left = -1;
    int64_t right = Size(t) - r;
    while (left + 1 < right) {
        int64_t mid = (left + right) / 2;
        if ((get_hash(t, l, l + mid) == get_hash(t, r, r + mid))) {
            left = mid;
        }
        else {
            right = mid;
        }
    }
    return left + 1;
}

int64_t main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    string s;
    cin >> s;
    int64_t n = s.size();
    deg[0] = 1;
    for (int64_t i = 1; i < n + 1; ++i) {
        deg[i] = (deg[i - 1] * P) % mod;
    }
    Tree* root = nullptr;
    for (int64_t i = 0; i < n; ++i) {
        Tree* new_char = new Tree(int64_t(s[i]));
        root = merge(root, new_char);
    }
    int64_t m;
    cin >> m;
    for (int64_t i = 0; i < m; i++) {
        int64_t type, l, r;
        cin >> type >> l >> r;
        l--;
        r--;
        if (type == 1) {
            rev(root, l, r);
        }
        else {
            cout << get_ans(root, l, r) << endl;
        }
    }
}
