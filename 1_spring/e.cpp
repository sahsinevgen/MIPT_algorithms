#include<iostream>
#include<set>

using namespace std;

int64_t id_ = 0;

struct List{
    List* L;
    List* R;
    int64_t l;
    int64_t r;
    int64_t id;
    bool isfree;
    List(int64_t l_, int64_t r_, bool isfree_) {
        L = R = nullptr;
        l = l_;
        r = r_;
        id = id_++;
        isfree = isfree_;
    }
};

bool f(pair<pair<int64_t, int64_t>, List*> a, pair<pair<int64_t, int64_t>, List*> b) {
    if (a.first.first != b.first.first)
        return a.first.first < b.first.first;
    if (a.first.second != b.first.second)
        return a.first.second > b.first.second;
    return a.second->id < b.second->id;
}

set<pair<pair<int64_t, int64_t>, List*>> w;

void del(List* l) {
    if (l->L != nullptr)
        l->L->R = l->R;
    if (l->R != nullptr)
        l->R->L = l->L;
    //delete l;
}

const int64_t MAXM = 2e5;
List* q[MAXM];

int main() {
    int64_t n, m;
    cin >> n >> m;
    List* root = new List(1, n, 1);
    w.insert({{n, -1}, root});
    w.insert({{0, 0}, nullptr});
    int64_t cnt = 0;
    while (m--) {
        int64_t i;
        cin >> i;
        if (i > 0) {
            auto it = *(w.rbegin());
            if (it.first.first < i) {
                cout << -1 << endl;
                q[cnt++] = nullptr;
            } else {
                cout << -it.first.second << endl;
                w.erase(it);
                if (it.first.first == i) {
                    it.second->isfree = 0;
                    q[cnt++] = it.second;
                    continue;
                }
                List* R = new List(it.second->l + i, it.second->r, 1);
                R->L = it.second;
                R->R = it.second->R;
                it.second->isfree = 0;
                it.second->r = it.second->l + i - 1;
                it.second->R = R;
                w.insert({{R->r - R->l + 1, -R->l}, R});
                q[cnt++] = it.second;
            }
        } else {
            cnt++;
            i = -i;
            i--;
            if (q[i] == nullptr)
                continue;
            if ((q[i]->L == nullptr || !q[i]->L->isfree) && (q[i]->R == nullptr || !q[i]->R->isfree)) {
                q[i]->isfree = 1;
                w.insert({{q[i]->r - q[i]->l + 1, -q[i]->l}, q[i]});
            } else if (q[i]->L == nullptr || !q[i]->L->isfree) {
                w.erase({{q[i]->R->r - q[i]->R->l + 1, -q[i]->R->l}, q[i]->R});
                q[i]->R->l = q[i]->l;
                w.insert({{q[i]->R->r - q[i]->R->l + 1, -q[i]->R->l}, q[i]->R});
                del(q[i]);
            } else if (q[i]->R == nullptr || !q[i]->R->isfree) {
                w.erase({{q[i]->L->r - q[i]->L->l + 1, -q[i]->L->l}, q[i]->L});
                q[i]->L->r = q[i]->r;
                w.insert({{q[i]->L->r - q[i]->L->l + 1, -q[i]->L->l}, q[i]->L});
                del(q[i]);
            } else {
                w.erase({{q[i]->L->r - q[i]->L->l + 1, -q[i]->L->l}, q[i]->L});
                w.erase({{q[i]->R->r - q[i]->R->l + 1, -q[i]->R->l}, q[i]->R});
                //q[i]->L->R = q[i]->R->R;
                q[i]->L->r = q[i]->R->r;
                w.insert({{q[i]->L->r - q[i]->L->l + 1, -q[i]->L->l}, q[i]->L});
                del(q[i]->R);
                del(q[i]);
                //w.insert({{q[i]->L->r - q[i]->L->l + 1, q[i]->L->l}, q[i]->L});
            }
        }
    }
}
