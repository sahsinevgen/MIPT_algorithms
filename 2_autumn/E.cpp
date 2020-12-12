#include<iostream>
#include<fstream>
#include<vector>
#include<queue>

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

int dinitsa(vector<vector<edge> > &g, int s, int t, int maxflow) {
    if (s == t) {
        exit(1);
    }
    int ans = 0;
    int n = g.size();
    vector<int> d(n);
    vector<int> id(n);
    while (true) {
        for (int i = 0; i < n; i++) {
            d[i] = -1;
        }
        if (!bfs(g, s, t, d)) {
            break;
        }
        for (int i = maxflow; i > 0; i >>= 1) {
            for (int j = 0; j < n; j++) {
                id[j] = 0;
            }
            while (int inc2 = dfs(g, s, t, d, id, i, s, maxflow)) {
                ans += inc2;
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

bool good(vector<vector<char> > &field,
         int i,
         int j) {
    int n = field.size();
    int m = field[0].size();
    return 0 <= i && i <= n && 0 <= j && j <= m && field[i][j] == '.';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int n, m, a, b;
    cin >> n >> m >> a >> b;
    vector<vector<edge> > g(n * m + 2);
    vector<vector<int> > rev_edges(n * m + 2);
    vector<vector<char> > field(n, vector<char>(m));
    int cnt_bad = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> field[i][j];
            if (field[i][j] == '.') {
                continue;
            }
            cnt_bad++;
            if (i != 0 && field[i - 1][j] != '.') {
                int start = i * m + j;
                int end = (i - 1) * m + j;
                if ((i + j) % 2) {
                    swap(start, end);
                }
                add_edge(g, rev_edges, start, end, 1);
            }
            if (j != 0 && field[i][j - 1] != '.') {
                int start = i * m + j;
                int end = i * m + j - 1;
                if ((i + j) % 2) {
                    swap(start, end);
                }
                add_edge(g, rev_edges, start, end, 1);
            }
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int now = i * m + j;
            if ((i + j + 1) % 2) {
                add_edge(g, rev_edges, n * m, now, 1);
            } else {
                add_edge(g, rev_edges, now, n * m + 1, 1);
            }
        }
    }
    for (int u = 0; u < g.size(); u++) {
        for (int i = 0; i < g[u].size(); i++) {
            g[u][i].rev = &(g[g[u][i].v][rev_edges[u][i]]);
        }
    }
    if (b * 2 <= a) {
        cout << cnt_bad * b << endl;
    } else {
        int x = dinitsa(g, n * m, n * m + 1, 1);
        cout << x * a + (cnt_bad - x * 2) * b << endl;
    }
}