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

int dinitsa(vector<vector<edge> > &g, int s, int t, int maxcap, int maxflow = -1) {
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
        for (int i = maxcap; i > 0; i >>= 1) {
            for (int j = 0; j < n; j++) {
                id[j] = 0;
            }
            while (int inc = dfs(g, s, t, d, id, i, s, maxcap)) {
                ans += inc;
                if (maxflow != -1 && ans > maxflow) {
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

void print(vector<vector<edge> > &g) {
    for (int u = 0; u < g.size(); u++) {
        cout << u + 1 << ": ";
        for (int i = 0; i < g[u].size(); i++) {
            // if (!g[u][i].cap) {
            //     continue;
            // }
            cout << g[u][i].v + 1 << " " << g[u][i].cur << "/" << g[u][i].cap << ", ";
        }
        cout << endl;
    }
    cout << "\n/////////////////////////////" << endl;
}

int inc(vector<vector<edge> > &g,
        int s,
        int t,
        int u,
        int i) {
    g[u][i].cap++;
    return dinitsa(g, s, t, 2000, 1);
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
        int change = -1 + dinitsa(g, s, t, 2000, 1);
        // cout << u + 1 << " change " << change << endl;
        return change;
    }
}



int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int tests;
    cin >> tests;
    while (tests--) {
        int n, m, a, b;
        cin >> n >> m;
        vector<vector<edge> > g(n + m + 2);
        vector<vector<int> > rev_edges(n + m + 2);
        int s = n + m;
        int t = n + m + 1;
        for (int u = 0; u < n; u++) {
            int v;
            vector<int> friends(m, 0);
            while (cin >> v) {
                v--;
                if (v == -1) {
                    break;
                }
                friends[v] = 1;
            }
            for (int i = 0; i < m; i++) {
                if (!friends[i]) {
                    add_edge(g, rev_edges, u, i + n, 1);
                }
            }
        }
        for (int u = 0; u < n; u++) {
            add_edge(g, rev_edges, s, u, 1);
        }
        for (int u = 0; u < m; u++) {
            add_edge(g, rev_edges, u + n, t, 1);
        }
        for (int u = 0; u < g.size(); u++) {
            for (int i = 0; i < g[u].size(); i++) {
                g[u][i].rev = &(g[g[u][i].v][rev_edges[u][i]]);
            }
        }
        // print(g);
        int flow = dinitsa(g, s, t, 1);
        // cout << "FLOW = " << flow << endl;
        int cnt_boys = n;
        int cnt_girls = m;
        vector<int> boys(n, 1);
        vector<int> girls(m, 1);
        for (int v = 0; v < m; v++) {
            if (g[v + n].back().remaind()) {
                continue;
            }
            int u;
            for (int i = 0; i < g[v + n].size(); i++) {
                if (g[v + n][i].remaind()) {
                    u = g[v + n][i].v;
                }
            }
            // cout << u + 1 << " " << v + 1 << " ";
            if (inc(g, s, t, v + n, g[v + n].size() - 1)) {
                girls[v] = 0;
                cnt_girls--;
                dec(g, s, t, v + n, g[v + n].size() - 1);
                // cout << "\n <-- kill girl" << endl;
            } else {
                dec(g, s, t, s, u);
                boys[u] = 0;
                cnt_boys--; 
                // cout << " <-- kill boy" << endl;
            }
            dec(g, s, t, v + n, g[v + n].size() - 1);
            // print(g);
        }
        cout << cnt_boys + cnt_girls << "\n" << cnt_boys << " " << cnt_girls << "\n";
        for (int i = 0; i < n; i++) {
            if (boys[i]) {
                cout << i + 1 << " ";
            }
        }
        cout << "\n";
        for (int i = 0; i < m; i++) {
            if (girls[i]) {
                cout << i + 1 << " ";
            }
        }
        cout << "\n" << "\n";
    }
}