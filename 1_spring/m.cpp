#include<bits/stdc++.h>

using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> g(n);
    vector<vector<int>> gr(n);
    for (int i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;
        x--;
        y--;
        g[x].push_back(y);
        gr[y].push_back(x);
    }
    vector<int> ans1(n, -1);
    vector<bool> is_end(n, true);
    for (int turn = 0; ; turn++) {
        queue<int> q;
        vector<int> used(n);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < g[i].size(); j++) {
                int v = g[i][j];
                if (ans1[v] == -1) {
                    ++used[i];
                }
            }
            if ((ans1[i] != -1) && (is_end[i])) {
                used[i] = -1;
            }
            else if (used[i] == 0) {
                q.push(i);
            }
        }
        if (q.empty()) {
            break;
        }
        vector<bool> checked(n);
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            ans1[v] = turn;
            for (int j = 0; j < gr[v].size(); j++) {
                int u = gr[v][j];
                if ((ans1[u] == -1) && (!checked[u])) {
                    checked[u] = true;
                    for (int k = 0; k < gr[u].size(); k++) {
                        int u2 = gr[u][k];
                        if (is_end[u2]) {
                            if (used[u2] != -1) {
                                --used[u2];
                                if (used[u2] == 0) {
                                    q.push(u2);
                                }
                            }
                        }
                    }
                }
            }
        }a
        for (int i = 0; i < n; ++i) {
            if ((ans1[i] == -1) && (is_end[i])) {
                is_end[i] = false;
                for (int j = 0; j < g[i].size(); j++) {
                    int v = g[i][j];
                    if (ans1[v] == turn) {
                         is_end[i] = true;
                         break;
                    }
                }
            }
        }
    }
    for (int i = 0; i < n; ++i) {
        cout << ans1[i] << ' ';
    }
    cout << endl;
}
