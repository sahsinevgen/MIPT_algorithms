#include<iostream>
#include<fstream>
#include<vector>
#include<queue>
#include<map>

using namespace std;

struct edge {
    int v;
    int cap;
    int cur;
    edge *rev;

    edge():v(0), cap(0), cur(0), rev(nullptr) {};
    edge(int v_, int cap_, edge* rev_ = nullptr): v(v_), cap(cap_), cur(0), rev(rev_) {};

    int remaind() {
        return cap - cur;
    }
};

int dfs(vector<vector<edge> > &g, int s, int t, vector<int> &d, vector<int> &id, int minflow, int u, int nowflow) {
    // cout << u + 1 << " " << nowflow << endl;
    if (u == t) {
        return nowflow;
    }
    for (; id[u] < g[u].size(); id[u]++) {
        if (g[u][id[u]].remaind() < minflow || d[u] + 1 != d[g[u][id[u]].v]) {
            continue;
        }
        int flow = dfs(g, s, t, d, id, minflow, g[u][id[u]].v, min(nowflow, g[u][id[u]].remaind()));
        if (flow) {
            g[u][id[u]].cur += flow;
            g[u][id[u]].rev->cur -= flow;
            return flow;
        }
    }
    return 0;
}

int bfs(vector<vector<edge> > &g, int s, int t, vector<int> &d) {
    queue<int> q;
    d[s] = 0;
    q.push(s);
    while (!q.empty() && d[t] == -1) {
        int now = q.front();
        q.pop();
        for (int i = 0; i < g[now].size(); i++) {
            int v = g[now][i].v;
            if (g[now][i].remaind() && d[v] == -1) {
                d[v] = d[now] + 1;
                q.push(v);
            }
        }
    }
    return d[t] != -1;
}

int dinitsa(vector<vector<edge> > &g, int s, int t, int maxcap, int maxflow = -1) {
    if (s == t) {
        cout << "SHIT" << endl;
        exit(1);
    }
    int ans = 0;
    int n = g.size();
    while (true) {
        vector<int> d(n, -1);
        if (!bfs(g, s, t, d)) {
            break;
        }
        for (int i = maxcap; i > 0; i >>= 1) {
            vector<int> id(n, 0);
            while (int inc = dfs(g, s, t, d, id, i, s, maxcap)) {
                ans += inc;
                if (maxflow != -1 && ans >= maxflow) {
                    return ans;
                }
            }
        }
    }
    return ans;
}

void add_edge(vector<vector<edge> > &g,
              vector<vector<int> > &rev_edges,
              int v,
              int u,
              int cap,
              int rev_cap = 0) {
    rev_edges[v].push_back(g[u].size());
    rev_edges[u].push_back(g[v].size());
    g[v].emplace_back(u, cap);
    g[u].emplace_back(v, rev_cap);
}

int inc(vector<vector<edge> > &g,
        int s,
        int t,
        int u,
        int i) {
    g[u][i].cap++;
    return dinitsa(g, s, t, 2000);
}

int dec(vector<vector<edge> > &g,
        int s,
        int t,
        int u,
        int i) {
    if (g[u][i].remaind()) {
        g[u][i].cap--;
        return 0;
    } else {
        int v = g[u][i].v;
        if (t != v) {
            dinitsa(g, t, v, 1, 1);
        }
        if (u != s) {
            dinitsa(g, u, s, 1, 1);
        }
        g[u][i].cap--;
        g[u][i].cur--;
        g[u][i].rev->cur++;
        return -1 + dinitsa(g, s, t, 2000);
    }
}

int main() {
    int n;
    cin >> n;
    if (n == 0) {
        return 0;
    }
    map<pair<int, int>, int> x;
    vector<vector<edge> > g(n, vector<edge>(0));
    int s, t, m;
    cin >>  m;
    s = 0;
    t = n - 1;
    vector<vector<int>> rev_edges(n, vector<int>(0));
    for (int i = 0; i < m; i++) {
        int u, v, cap;
        cin >> u >> v >> cap;
        u--;
        v--;
        add_edge(g, rev_edges, u, v, cap);
        x[make_pair(u, v)] = g[u].size() - 1;
        // g[u].back().rev = &(g[v].back());
        // g[v].back().rev = &(g[u].back());
    }
    for (int i = 0; i < g.size(); ++i) {
        for (int j = 0; j < g[i].size(); ++j) {
            g[i][j].rev = &(g[g[i][j].v][rev_edges[i][j]]);
        }
    }
    int now_flow = dinitsa(g, s, t, 2000);
    cout << now_flow << endl;
    int k;
    cin >> k;
    while (k--) {
        int type, v, u;
        cin >> type >> u >> v;
        u--;
        v--;
        if (type == 1) {
            int i = x[make_pair(u, v)];
            now_flow += inc(g, s, t, u, i);
            cout << now_flow << endl;
        } else {
            int i = x[make_pair(u, v)];
            now_flow += dec(g, s, t, u, i);
            cout << now_flow << endl;
        }
    }
}