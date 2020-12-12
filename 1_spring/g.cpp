#include<bits/stdc++.h>

using namespace std;

struct Tree {
    int64_t val;
    int64_t update_type;
    int64_t update_val;
    int64_t sum;
    int64_t y;
    int64_t size;
    Tree* L;
    Tree* R;

    Tree() {}
    Tree(int64_t x) {
        sum = x;
        val = x;
        y = rand();
        L = R = nullptr;
        update_type = 1;
        update_val = 0;
        size = 1;
    }
};

int64_t Size(Tree* t) {
    if (t == nullptr) {
        return 0;
    }
    return t->size;
}

void f(Tree* t) {
    if (t == nullptr) {
        return;
    }
    t->sum = t->val;
    if (t->L != nullptr) {
        t->sum += (t->L->sum) * (t->L->update_type) + (Size(t->L)) * (t->L->update_val);
    }
    if (t->R != nullptr) {
        t->sum += (t->R->sum) * (t->R->update_type) + (Size(t->R)) * (t->R->update_val);
    }
}

void update(Tree* t) {
    if (t == nullptr) {
        return;
    }
    t->size = Size(t->L) + Size(t->R) + 1;
    f(t);
}

void push_t(Tree* t) {
    if (t == nullptr) {
        return;
    }
    int64_t type = t->update_type;
    if (t->L != nullptr) {
        (t->L->update_val) = (t->L->update_val) * type + (t->update_val);
        (t->L->update_type) = min((t->L->update_type), (t->update_type));
    }
    if (t->R != nullptr) {
        (t->R->update_val) = (t->R->update_val) * type + (t->update_val);
        (t->R->update_type) = min((t->R->update_type), (t->update_type));
    }
    t->sum = (t->val) * type  + (t->update_type) * (Size(t));
    t->val = (t->val) * type + (t->update_val);
    t->update_type = 1;
    t->update_val = 0;
}

pair<Tree*, Tree*> split(Tree* t, int64_t k) {
    if (t == nullptr) {
        return {nullptr, nullptr};
    }
    push_t(t);
    pair<Tree*, Tree*> tt;
    int64_t lsz = Size(t->L);
    if (lsz < k) {
        tt = split(t->R, k - lsz - 1);
        t->R = tt.first;
        update(t);
        f(t);
        return {t, tt.second};
    }
    else {
        tt = split(t->L, k);
        t->L = tt.second;
        update(t);
        return {tt.first, t};
    }
}

Tree* merge_t(Tree* l, Tree* r) {
    push_t(l);
    push_t(r);
    if (l == nullptr) {
        update(r);
        return r;
    }
    if (r == nullptr) {
        update(l);
        return l;
    }
    if (l->y > r->y) {
        l->R = merge_t(l->R, r);
        update(l);
        return l;
    }
    else {
        r->L = merge_t(l, r->L);
        update(r);
        return r;
    }
}

Tree* upd_seg(Tree* t, int64_t l, int64_t r, int64_t type, int64_t val) {
    pair<Tree*, Tree*> tt = split(t, r);
    Tree* t3 = tt.second;
    Tree* t2 = tt.first;
    tt = split(t2, l - 1);
    Tree* t1 = tt.first; t2 = tt.second;
    if (t2) {
        push_t(t2);
        t2->update_type = type;
        t2->update_val = (t2->update_val) * type + val;
        push_t(t2);
    }
    t2 = merge_t(t1, t2);
    t = merge_t(t2, t3);
    return t;
}

pair<int64_t, Tree*> get_sum_lr(Tree* t, int64_t l, int64_t r) {
    pair<Tree*, Tree*> tt = split(t, r);
    Tree* t3 = tt.second;
    Tree* t2 = tt.first;
    tt = split(t2, l - 1);
    Tree* t1 = tt.first;
    t2 = tt.second;
    int64_t answer = t2->sum;
    t2 = merge_t(t1, t2);
    t = merge_t(t2, t3);
    return {answer, t};
}

void upd_seg5(vector<Tree*> &a, int64_t l, int64_t r, int64_t type, int64_t val) {
    for (int64_t i = 0; i < 5; i++) {
        int64_t cl = (i < l % 5) + l / 5 + 1;
        int64_t cr = (i <= r % 5) + r / 5 ;
        if (cl <= cr) {
            a[i] = upd_seg(a[i], cl, cr, type, val);
        }
    }
}

void insert5(vector<Tree*> &a, int64_t pos, int64_t val) {
    int64_t left = pos / 5;
    Tree* cmp = nullptr;
    int64_t i = pos % 5;
    for (int64_t j = 0; j < 5; j++, i = (i + 1) % 5) {
        pair<Tree*, Tree*> tt = split(a[i], left);
        Tree* t1 = tt.first;
        Tree* t2 = tt.second;
        a[i] = merge_t(t1, cmp);
        cmp = t2;
        if (i == 4) left++;
    }
    Tree* t = new Tree(val);
    cmp = merge_t(t, cmp);
    a[pos % 5] = merge_t(a[pos % 5], cmp);
}

int64_t get_sum5(vector<Tree*> &a, int64_t l, int64_t r) {
    int64_t answer = 0;
    for (int64_t i = 0; i < 5; ++i) {
        int64_t cl = (i < l % 5) + l / 5 + 1;
        int64_t cr = (i <= r % 5) + r / 5;
        if (cl > cr) continue;
        pair<int64_t, Tree*> res = get_sum_lr(a[i], cl, cr);
        a[i] = res.second;
        answer += res.first;
    }
    return answer;
}

void cycle5(vector<Tree*> &a, int64_t l, int64_t r) {
    int64_t len = (r - l + 1) / 5;
    int64_t left = l / 5;
    Tree* cmp = nullptr;
    int64_t i = l % 5;
    for (int64_t j = 0; j < 5; ++j, i = (i + 1) % 5) {
        pair<Tree*, Tree*> tt = split(a[i], left);
        Tree* t1 = tt.first;
        Tree* t2 = tt.second;
        tt = split(t2, len);
        t2 = tt.first;
        Tree* t3 = tt.second;
        t1 = merge_t(t1, cmp);
        a[i] = merge_t(t1, t3);
        cmp = t2;
        if (i == 4) {
            left++;
        }
    }
    pair<Tree*, Tree*> tt = split(a[l % 5], left - 1);
    tt.first = merge_t(tt.first, cmp);
    a[l % 5] = merge_t(tt.first, tt.second);
}

int main() {
    int64_t m;
    cin >> m;
    vector<Tree*> roots(5, nullptr);
    for (int64_t i = 0; i < m; ++i) {
        char type;
        cin >> type;
        if (type == '0') {
            int64_t l, r;
            int64_t val;
            cin >> l >> r >> val;
            l--;
            r--;
            upd_seg5(roots, l, --r, 0, val);
        }
        if (type == '1') {
            int64_t l, r;
            int64_t val;
            cin >> l >> r >> val;
            l--;
            r--;
            upd_seg5(roots, l, r, 1, val);
        }
        if (type == '2') {
            int64_t id;
            int64_t val;
            cin >> id >> val;
            id--;
            insert5(roots, id, val);
        }
        if (type == '3') {
            int64_t l, r;
            cin >> l >> r;
            l--;
            r--;
            cout << get_sum5(roots, l, r) << endl;
        }
        if (type == 'X') {
            int64_t l, r;
            cin >> l >> r;
            l--;
            r--;
            cycle5(roots, l, r);
        }
    }
}
