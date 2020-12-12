#include<iostream>
#include<random>
#include<vector>

using namespace std;

struct Tree{
    Tree* L;
    Tree* R;
    Tree* parent;
    int64_t val;
    int64_t y;
    int64_t size;
    bool reversed;
    bool cycled;
    Tree(int64_t val_ = 0){
        L = R = parent = nullptr;
        val = val_;
        size = 1;
        y = rand();
        reversed = false;
        cycled = false;
    }
};

const int64_t MAXN = 1e5;
Tree* q[MAXN];

int64_t Size(Tree* t) {
    if (t == nullptr)
        return 0;
    return t->size;
}

void push(Tree* t) {
    if (t == nullptr)
        return;   
    if (!t->reversed)
        return;
    t->reversed = false;
    swap(t->L, t->R);
    if (t->L != nullptr) {
        t->L->reversed ^= 1;
        t->L->parent = t;
    }
    if (t->R != nullptr) {
        t->R->reversed ^= 1;
        t->R->parent = t;
    }
}

void update(Tree* t) {
    if (t == nullptr)
        return;
    if (t->L != nullptr)
        t->L->parent = t;
    if (t->R != nullptr)
        t->R->parent = t;
    t->size = Size(t->L) + Size(t->R) + 1;
}

pair<Tree*, Tree*> split(Tree* t, int64_t k) {
    push(t);
    t->parent = nullptr;
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
    push(l);
    push(r);
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

void add_road(Tree* a, Tree* b) {
    Tree* a1 = a;
    Tree* b1 = b;
    vector<Tree*> apath, bpath;
    while (a1->parent != nullptr) {
        apath.push_back(a1);
        a1 = a1->parent;
    }
    //apath.push_back(a1);
    while (b1->parent != nullptr) {
        bpath.push_back(b1);
        b1 = b1->parent;
    }
    //bpath.push_back(b1);
    if (a1 == b1) {
        a1->cycled = true;
        return;
    }
    push(a1);
    push(b1);
    Tree* last = a1;
    int64_t ansa = Size(a->L);
    for (int64_t i = apath.size() - 1; i >= 1; i--) {
        if (last->R == apath[i - 1] && last->L != nullptr) {
            ansa += last->L->size;
        }
        ansa++;
        last = apath[i];
        push(apath[i]);
    }
    if (ansa != a1->size - 1) {
        a1->reversed = true;
    }
    last = b1;
    int64_t ansb = Size(b->L);
    for (int64_t i = bpath.size() - 1; i >= 1; i--) {
        if (last->R == bpath[i - 1] && last->L != nullptr) {
            ansb += last->L->size;
        }
        ansb++;
        last = bpath[i];
        push(bpath[i]);
    }
    if (ansb != 0) {
        b1->reversed = true;
    }
    merge(a1, b1);
}

void cut_road(Tree* a, Tree* b) {
    Tree* a1 = a;
    Tree* b1 = b;
    vector<Tree*> apath, bpath;
    while (a1->parent != nullptr) {
        apath.push_back(a1);
        a1 = a1->parent;
        cout << a1->val << endl;
    }
    //apath.push_back(a1);
    cout << 123 << endl;
    while (b1->parent != nullptr) {
        bpath.push_back(b1);
        b1 = b1->parent;
        cout << b1->val << endl;
    }
    //bpath.push_back(b1);
    if (a1->cycled) {
        a1->cycled = false;
        return;
    }
    cout << 123 << endl;
    Tree* last = a1;
    int64_t ansa = Size(a->L);
    for (int64_t i = apath.size() - 1; i >= 1; i--) {
        if (last->R == apath[i - 1] && last->L != nullptr) {
            ansa += last->L->size;
        }
        ansa++;
        last = apath[i];
        push(apath[i]);
    }
    last = b1;
    int64_t ansb = Size(b->L);
    for (int64_t i = bpath.size() - 1; i >= 1; i--) {
        if (last->R == bpath[i - 1] && last->L != nullptr) {
            ansb += last->L->size;
        }
        ansb++;
        last = bpath[i];
        push(bpath[i]);
    }
    split(a1, max(ansa, ansb));
}

int64_t get_ans(Tree* a, Tree* b) {
    Tree* a1 = a;
    Tree* b1 = b;
    vector<Tree*> apath, bpath;
    while (a1->parent != nullptr) {
        apath.push_back(a1);
        a1 = a1->parent;
    }
    apath.push_back(a1);
    while (b1->parent != nullptr) {
        bpath.push_back(b1);
        b1 = b1->parent;
    }
    bpath.push_back(b1);
    if (a1 != b1) {
        return -1;
    }
    Tree* last = a1;
    int64_t ansa = Size(a->L);
    for (int64_t i = apath.size() - 1; i >= 1; i--) {
        last = apath[i];
        push(apath[i]);
        if (last->R == apath[i - 1] && last->L != nullptr) {
            ansa += last->L->size;
        }
        ansa++;
        cout << (last->R == nullptr ? -1 : last->R->val) << " " << (last->L == nullptr ? -1 : last->L->val) << endl;
        cout << Size(apath[i]) << " " << Size(apath[i]->L) << " " << Size(apath[i]->R) << endl;
        cout << apath[i]->val << "size = " << Size(apath[i]) << "   ansa = " << ansa << endl;
    }
    last = b1;
    int64_t ansb = Size(b->L);
    for (int64_t i = bpath.size() - 1; i >= 1; i--) {
        last = bpath[i];
        push(bpath[i]);
        if (last->R == bpath[i - 1] && last->L != nullptr) {
            ansb += last->L->size;
        }
        ansb++;
        cout << Size(bpath[i]) << " " << Size(bpath[i]->L) << " " << Size(bpath[i]->R) << endl;
        cout << bpath[i]->val << "size = " << Size(bpath[i]) << "   ansb = " << ansb << endl;
    }
    int64_t ans = max(abs(ansa - ansb) - 1, int64_t(0));
    cout << "anss" << ansa << " " << ansb << " " << ans << endl;
    if (a1->cycled) {
        return min(ans, a1->size - ans - int64_t(2));
    } else {
        return ans;
    }
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
    int64_t n, m, k;
    cin >> n >> m >> k;
    vector<Tree*> qq(n);
    for (int i = 0; i < n; i++) {
        qq[i] = new Tree(i);
    }
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        a--;
        b--;
        add_road(qq[a], qq[b]);
        for (int i = 0; i < n; i++) {
            cout<< (qq[i]->parent == nullptr ? -1 : qq[i]->parent->val) << " ";
        }
        cout << endl;
    }
    for (int i = 0; i < k; i++) {
        char c;
        cin >> c;
        int a, b;
        cin >> a >> b;
        a--;
        b--;
        if (c == '-') {
            cut_road(qq[a], qq[b]);
        }
        if (c == '+') {
            add_road(qq[a], qq[b]);
        }
        if (c == '?') {
            cout << get_ans(qq[a], qq[b]) << endl;
        }
        for (int i = 0; i < n; i++) {
            cout<< (qq[i]->parent == nullptr ? -1 : qq[i]->parent->val) << " ";
        }
        cout << endl;
    }
}
