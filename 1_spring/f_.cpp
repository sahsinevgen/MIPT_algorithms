#include <bits/stdc++.h>

using namespace std;

struct Tree {
    Tree* L;
    Tree* R;
    int64_t x;
    int64_t y;
    int64_t size;
    bool reversed;
    bool cycle;
    Tree(int64_t x_) {
        x = x_;
        size = 1;
        y = rand();
    }
};

const int64_t N = 2e5 + 20;
Tree* pr[N];

void push(Tree* t) {
    if (t == nullptr || !t->reversed) {
        return;
    }
    if (t->L != nullptr) {
        t->L->reversed ^= 1;
    }
    if (t->R != nullptr) {
        t->R->reversed ^= 1;
    }
    t->reversed = false;
    swap(t->R, t->L);
}

int64_t Size(Tree* t) {
    if (t == nullptr) {
        return 0;
    }
    return t->size;
}

void upd(Tree* t) {
    if (t == nullptr) {
        return;
    }
    t->size = Size(t->L) + Size(t->R) + 1;

    pr[t->x] = t;
    if (t->L != nullptr) {
        pr[t->L->x] = t;
    }
    if (t->R != nullptr) {
        pr[t->R->x] = t;
    }
}

Tree* merge(Tree* l, Tree* r) {
    push(l);
    push(r);
    if (l == nullptr) {
        return r;
    }
    if (r == nullptr) {
        return l;
    }
    if (l->y > r->y) {
        l->R = merge(l->R, r);
        upd(l);
        return l;
    }
    else {
        r->L = merge(l, r->L);
        upd(r);
        return r;
    }
}

Tree* par(int64_t a) {
    Tree* ret = pr[a];
    for (; pr[ret->x]->x != ret->x; ret = pr[ret->x]) {}
    return ret;
}

void reversed(int64_t a) {
    Tree* v = par(a);
    push(v);
    v->reversed ^= 1;
}

int64_t get_left(Tree* t) {
    push(t);
    if (t->L == nullptr) {
        return t->x;
    }
    return get_left(t->L);
}

int64_t get_right(Tree* t) {
    push(t);
    if (t->R == nullptr) {
        return t->x;
    }
    return get_right(t->R);
}

void add(int64_t a, int64_t b) {
    if (par(a) == par(b)) {
        par(a)->cycle = true;
        return;
    }
    swap(a, b);
    int64_t r_a = get_right(par(a));
    int64_t l_b = get_left(par(b));
    if (r_a != a) {
        reversed(a);
    }
    if (l_b != b) {
        reversed(b);
    }
    merge(par(a), par(b));
}

int64_t pos(int64_t x) {
    push(pr[x]);
    if (pr[x]->x == x) {
        return Size(pr[x]->L);
    }
    Tree* cur = pr[x]->L;
    if ((pr[x]->L == nullptr) || (pr[x]->L->x != x)) {
        cur = pr[x]->R;
    }
    vector<Tree*> v;
    while (cur->x != pr[cur->x]->x) {
        v.emplace_back(cur);
        cur = pr[cur->x];
    }
    v.emplace_back(cur);
    reverse(v.begin(), v.end());
    int64_t res = 0;
    for (int64_t i = 0; i < (int64_t)v.size() - 1; ++i) {
        push(v[i]);
        if (v[i]->L != v[i + 1]) {
            res += 1 + Size(v[i]->L);
        }
    }
    push(v.back());
    res += Size(v.back()->L);
    return res;
}

pair<Tree*, Tree*> split(Tree* t, int64_t x) {
    push(t);
    if (t == nullptr) {
        return {nullptr, nullptr};
    }
    if (Size(t->L) < x) {
        auto p = split(t->R, x - Size(t->L) - 1);
        t->R = p.first;
        upd(t);
        upd(p.second);
        return {t, p.second};
    }
    else {
        auto p = split(t->L, x);
        t->L = p.second;
        upd(t);
        upd(p.first);
        return {p.first, t};
    }
}

void cut(int64_t a, int64_t b) {
    if (pos(a) > pos(b)) {
        swap(a, b);
    }
    if (par(a)->cycle) {
        par(a)->cycle = false;
        if ((a == get_left(par(a))) && (b == get_right(par(a)))) {
            return;
        }
        auto z = split(par(a), pos(a) + 1);
        add(get_left(z.first), get_right(z.second));
    }
    else {
        split(par(a), pos(a) + 1);
    }
}

int64_t ans(int64_t a, int64_t b) {
    if (a == b) {
        return 0;
    }
    if (par(a) != par(b)) {
        return -1;
    }
    int64_t res = abs(pos(a) - pos(b)) - 1;
    if (par(a)->cycle) {
        res = min(res, Size(par(a)) - res - 2);
    }
    return res;
}

int main() {
    int64_t n, m, q;
    cin >> n >> m >> q;
    for (int64_t i = 1; i <= n; ++i) {
        pr[i] = new Tree(i);
    }
    for (int64_t i = 0; i < m; ++i) {
        int64_t a, b;
        cin >> a >> b;
        add(a, b);
    }
    for (int64_t i = 0; i < q; ++i) {
        char type;
        int64_t a, b;
        cin >> type >> a >> b;
        if (type == '+') {
            add(a, b);
        }
        else if (type == '-') {
            cut(a, b);
        }
        else {
            cout << ans(a, b) << endl;
        }
    }
}
