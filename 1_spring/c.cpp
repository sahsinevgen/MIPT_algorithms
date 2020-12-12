#include<iostream>
#include<pair>

using namespace std;

struct Tree{
    int64_t L;
    int64_t R;
    int64_t cnt;
    Tree(){
        L = R = -1;
        cnt = 0;
    }
};

int64_t INF = 1'000'000'010;
int64_t last = 0;
Tree T[6'000'000];

int64_t get_cnt(int64_t t) {
    if (t == -1)
        return 0;
    return T[t].cnt;
}

void add(int64_t t, int64_t i, int64_t x, int64_t l, int64_t r) {
    //cout << i << " " << x << " " << l << " " << r << " " << endl;
    if (l == r) {
        T[t].cnt += x;
        return;
    }
    int64_t m = (l + r) / 2;
    if (i > m) {
        if (T[t].R == -1)
            T[t].R = last++;
        add(T[t].R, i, x, m + 1, r);
        T[t].cnt = get_cnt(T[t].L) + get_cnt(T[t].R);
    }
    else {
        if (T[t].L == -1)
            T[t].L = last++;
        add(T[t].L, i, x, l, m);
        T[t].cnt = get_cnt(T[t].L) + get_cnt(T[t].R);
    }
}

int64_t ans(int64_t t, int64_t k, int64_t l, int64_t r) {
    //cout << k << " " << l << " " << r << " " << get_cnt(t) << " " << get_cnt(t->L) << " " << get_cnt(t->R) << endl;
    if (l == r)
        return l;
    int64_t m = (l + r) / 2;
    int64_t r_cnt = get_cnt(T[t].R);
    if (r_cnt > k) {
        if (T[t].R == -1)
            T[t].R = last++;
        return ans(T[t].R, k, m + 1, r);
    } else {
        if (T[t].L == -1)
            T[t].L = last++;
        return ans(T[t].L, k - r_cnt, l, m);
    }
}

int main() {
    int64_t root = last++;
    int64_t n;
    cin >> n;
    for (int64_t i = 0; i < n; i++) {
        while (last > 3'000'000);
        int64_t type;
        cin >> type;
        if (type != 0) {
            int64_t x;
            cin >> x;
            add(root, x + INF, type, 0, INF*2);
        }
        else {
            int64_t x;
            cin >> x;
            cout << ans(root, x-1, 0, INF*2) - INF << endl;
        }
    }
}
