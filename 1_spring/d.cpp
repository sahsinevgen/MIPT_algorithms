#include<iostream>
#include<random>

using namespace std;

struct Tree{
    Tree* L;
    Tree* R;
    int64_t val;
    int64_t y;
    int64_t size;
    Tree(int64_t val_ = 0){
        L = R = nullptr;
        val = val_;
        size = 1;
        y = rand();
    }
};

int64_t Size(Tree* t) {
    if (t == nullptr)
        return 0;
    return t->size;
}

void update(Tree* t) {
    if (t == nullptr)
        return;
    t->size = Size(t->L) + Size(t->R) + 1;
}

pair<Tree*, Tree*> split(Tree* t, int64_t k) {
    if (t == nullptr) 
        return pair<Tree*, Tree*>(nullptr, nullptr);
    int64_t l = Size(t->L);
    if (l >= k) {
        pair<Tree*, Tree*> q = split(t->L, k);
        t->L = q.second;
        update(t);
        return pair<Tree*, Tree*>(q.first, t);
    } else {
        pair<Tree*, Tree*> q = split(t->R, k - l - 1);
        t->R = q.first;
        update(t);
        return pair<Tree*, Tree*>(t, q.second);
    }
}

Tree* merge(Tree* l, Tree* r) {
    if (l == nullptr)
        return r;
    if (r == nullptr)
        return l;
    if (l->y > r->y) {
        l->R = merge(l->R, r);
        update(l);
        return l;
    } else {
        r->L = merge(l, r->L);
        update(r);
        return r;
    }
}

Tree* GO(Tree* t, int64_t l, int64_t r) {
    pair<Tree*, Tree*> q = split(t, l);
    pair<Tree*, Tree*> w = split(q.second, r - l + 1);
    t = merge(w.first, q.first);
    t = merge(t, w.second);
    return t;
}

Tree* add(Tree* t, int64_t val) {
    Tree* t2 = new Tree(val);
    return merge(t, t2);
}

void print(Tree* t) {
    if (t == nullptr)
        return;
    print(t->L);
    cout << t->val + 1 << " ";
    print(t->R);
}

int main() {
    int n, m;
    Tree* root = nullptr;
    cin >> n >> m;
    for (int i = 0; i < n; i++)
        root = add(root, i);
    while (m--) {
        int l, r;
        cin >> l >> r;
        root = GO(root, l - 1, r - 1);
    }
    print(root);
    cout << endl;
}
