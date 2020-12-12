#include<iostream>
#include<fstream>
#include<vector>
#include<queue>

using namespace std;

struct edge {
    int64_t v;
    int64_t cap;
    int64_t cur;
    edge *rev;

    edge():v(0), cap(0), cur(0), rev(nullptr) {};
    edge(int64_t v_, int64_t cap_, edge* rev_ = nullptr): v(v_), cap(cap_), cur(0), rev(rev_) {};

    int64_t remaind() {
        return cap - cur;
    }
};

int64_t dfs(vector<vector<edge> > &g, int64_t s, int64_t t, vector<int64_t> &d, vector<int64_t> &id, int64_t minflow, int64_t u, int64_t nowflow) {
    // cout << u + 1 << " " << nowflow << endl;
    if (u == t) {
        return nowflow;
    }
    for (; id[u] < g[u].size(); id[u]++) {
        if (g[u][id[u]].remaind() < minflow || d[u] + 1 != d[g[u][id[u]].v]) {
            continue;
        }
        int64_t flow = dfs(g, s, t, d, id, minflow, g[u][id[u]].v, min(nowflow, g[u][id[u]].remaind()));
        if (flow) {
            g[u][id[u]].cur += flow;
            g[u][id[u]].rev->cur -= flow;
            return flow;
        }
    }
    return 0;
}

int64_t bfs(vector<vector<edge> > &g, int64_t s, int64_t t, vector<int64_t> &d) {
    queue<int64_t> q;
    d[s] = 0;
    q.push(s);
    while (!q.empty() && d[t] == -1) {
        int64_t now = q.front();
        q.pop();
        for (int64_t i = 0; i < g[now].size(); i++) {
            int64_t v = g[now][i].v;
            if (g[now][i].remaind() && d[v] == -1) {
                d[v] = d[now] + 1;
                q.push(v);
            }
        }
    }
    return d[t] != -1;
}

int64_t dinitsa(vector<vector<edge> > &g, int64_t s, int64_t t, int64_t maxflow) {
    if (s == t) {
        exit(1);
    }
    int64_t ans = 0;
    int64_t n = g.size();
    while (true) {
        vector<int64_t> d(n, -1);
        if (!bfs(g, s, t, d)) {
            break;
        }
        for (int64_t i = maxflow; i > 0; i >>= 1) {
            vector<int64_t> id(n, 0);
            while (int64_t inc2 = dfs(g, s, t, d, id, i, s, maxflow)) {
                ans += inc2;
            }
        }
    }
    return ans;
}

int main() {
    while(1) {
        int64_t n;
        cin >> n;
        if (n == 0) {
            return 0;
        }
        vector<vector<edge> > g(n, vector<edge>(0));
        int64_t s, t, m;
        cin >> s >> t >> m;
        s--;
        t--;
        vector<vector<int64_t>> temp_g(n, vector<int64_t>(0));
        for (int64_t i = 0; i < m; i++) {
            int64_t u, v, cap;
            cin >> u >> v >> cap;
            u--;
            v--;
            temp_g[u].push_back(g[v].size());
            temp_g[v].push_back(g[u].size());
            g[u].emplace_back(v, cap);
            g[v].emplace_back(u, cap);
            // g[u].back().rev = &(g[v].back());
            // g[v].back().rev = &(g[u].back());
        }
        for (int64_t i = 0; i < g.size(); ++i) {
            for (int64_t j = 0; j < g[i].size(); ++j) {
                g[i][j].rev = &(g[g[i][j].v][temp_g[i][j]]);
            }
        }
        cout << dinitsa(g, s, t, 2000) << endl;
    }
}