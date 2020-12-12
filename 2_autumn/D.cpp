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

void custom_dfs(vector<vector<edge> > &g, int s1, int u, string &s, string &t) {
    for (int i = 0; i < g[u].size(); i++) {
        if (!g[u][i].remaind()) {
            continue;
        }
        int v = g[u][i].v;
        if (v != s1 && (v < s.size() && s[v] == '?' || v >= s.size() && t[v - s.size()] == '?')) {
            if (v < s.size()) {
                s[v] = '0';
            } else {
                t[v - s.size()] = '0';
            }
            custom_dfs(g, s1, v, s, t);
        }
    }
}

int main() {
    string s, t;
    cin >> s >> t;
    int n = s.size() + t.size() + 2;
    int s1 = n - 2;
    int t1 = n - 1;
    vector<vector<edge> > g(n, vector<edge>(0));
    vector<vector<int>> rev_edges(n, vector<int>(0));
    for (int i = 0; i < n - 2; i++) {
        add_edge(g, rev_edges, s1, i, 0);
        add_edge(g, rev_edges, i, t1, 0);
    }
    for (int j = 0; j < t.size(); j++) {
        for (int i = j; i < s.size() - t.size() + 1 + j; i++) {
            if (s[i] == '0' && t[j] == '?') {
                g[s1][j + s.size()].cap++;
            }
            if (s[i] == '1' && t[j] == '?') {
                g[j + s.size()][1].cap++;
            }
            if (s[i] == '?' && t[j] == '0') {
                g[s1][i].cap++;
            }
            if (s[i] == '?' && t[j] == '1') {
                g[i][1].cap++;
            }
            if (s[i] == '?' && t[j] == '?') {
                add_edge(g, rev_edges, i, j + s.size(), 1, 1);
            }
        }
    }
    for (int i = 0; i < g.size(); i++) {
        for (int j = 0; j < g[i].size(); j++) {
            g[i][j].rev = &(g[g[i][j].v][rev_edges[i][j]]);
        }
    }
    // cout << dinitsa(g, s1, t1, 1) << endl;
    // for (int i = 0; i < g.size(); i++) {
    //     if (i < s.size() && s[i] != '?' || i < n - 2 && i >= s.size() && t[i - s.size()] != '?') {
    //         continue;
    //     }
    //     for (int j = 0; j < g[i].size(); j++) {
    //         cout << i + 1 << " " << g[i][j].v + 1 << " " << g[i][j].cur << " / " << g[i][j].cap << endl;
    //     }
    // }
    dinitsa(g, s1, t1, 1);
    custom_dfs(g, s1, s1, s, t);
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '?') {
            s[i] = '1';
        }
    }
    for (int i = 0; i < t.size(); i++) {
        if (t[i] == '?') {
            t[i] = '1';
        }
    }
    int ans = 0;
    for (int j = 0; j < t.size(); j++) {
        for (int i = j; i < s.size() - t.size() + 1 + j; i++) {
            if (s[i] != t[j]) {
                ans++;
            }
        }
    }
    cout << ans << "\n" << s << "\n" << t << endl;
    // for (int i = 0; i < n - 2; i++) {
    //     for (int j = 0; j < g[i].size(); j++) {

    //     }
    // }


}