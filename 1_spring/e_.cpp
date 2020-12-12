#include<iostream>
#include<set>
#include<random>

using namespace std;

int64_t cnt = 0;

struct List {
    List* L;
    List* R;
    int64_t l;
    int64_t r;
    int64_t i;
    bool isfree;
    List(int64_t l_ = 0, int64_t r_ = 0, bool isfree_= 1) {
        L = R = nullptr;
        l = l_;
        r = r_;
        i = cnt++;
        isfree = isfree_;
    }
};

void del(List* t) {
    if (t->L != nullptr)
        t->L->R = t->R;
    if (t->R != nullptr)
        t->R->L = t->L;
}

int64_t len(List* a) {
    if (a == nullptr)
        return -1;
    return a->r - a->l + 1;
}

List* q[5'000'00];
//List* qq[9'000'00];
//int64_t cnt = 0;

bool cmp(List* a, List* b) {
    //List* a = qq[ai];
    //List* b = qq[bi];
    if (len(a) != len(b))
        return len(a) < len(b);
    if (a->l != b->l)
        return a->l > b->l;
    return a->i < b->i;
}

int main() {
    int64_t n, m;
    cin >> n >> m;
    List* root = new List(1, n, 1);
    set<List*, bool(*)(List*, List*)> w(cmp);
    w.insert(root);
    for (int64_t i = 0; i < m; i++) {
        int64_t x;
        cin >> x;
        if (x > 0) {
            //cout << "i = " << i << endl;
            if (w.empty()) {
                cout << -1 << endl;
                q[i] = nullptr;
                continue;
            }
            List* t = *(w.rbegin());
            //cout << "wsize = " << w.size() << " " << len(t) << " " << t->l << " " << t->r << endl;
            //List* t = qq[ti];
            if (len(t) < x) {
                cout << -1 << endl;
                q[i] = nullptr;
            } else {
                cout << t->l << endl;
                w.erase(t);
                q[i] = t;
                t->isfree = 0;
                if (len(t) == x) {
                    continue;
                }
                List* t2 = new List(t->l + x, t->r, 1);
                //qq[cnt++] = t2;
                if (t->R != nullptr)
                    t->R->L = t2;
                t2->R = t->R;
                t2->L = t;
                t->R = t2;
                t->r = t->l + x - 1;
                w.insert(t2);
            }
        } else {
            x = -x;
            x--;
            //cout << "x = " << x << endl;
            //int64_t ii = q[x];
            List* t = q[x];
            //cout << "cont" << endl;
            if (t == nullptr)
                continue;
            //cout << "inue" << endl;
            if ((t->L == nullptr || !(t->L->isfree)) && (t->R == nullptr || !(t->R->isfree))) {
                t->isfree = 1;
                w.insert(t);
            } else if (t->L == nullptr || !(t->L->isfree)) {
                w.erase(t->R);
                t->R->l = t->l;
                del(t);
                w.insert(t->R);
            } else if (t->R == nullptr || !(t->R->isfree)) {
                w.erase(t->L);
                t->L->r = t->r;
                del(t);
                w.insert(t->L);
            } else {
                w.erase(t->L);
                w.erase(t->R);
                t->L->r = t->R->r;
                del(t->R);
                del(t);
                w.insert(t->L);
            }
            q[x] = nullptr;
            //cout << "wsize = " << w.size() << endl;
        }
    }
}

