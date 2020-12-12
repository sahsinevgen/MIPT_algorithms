#include<bits/stdc++.h>

using namespace std;

vector<pair<int, set<int>>> calculate_game() {
}

int main() {
    vector<pair<int, set<int>>> ans[2];
    for (int ii = 0; ii < 2; ii++) {
        int n, m;
        cin >> n >> m;
        vector<vector<int>> g(n);
        vector<vector<int>> gr(n);
        for (int i = 0; i < m; i++) {
            int x, y;
            cin >> x >> y;
            x--, y--;
            g[x].push_back(y);
            gr[y].push_back(x);
        }
        vector<int> ans1(n, -1);
        vector<bool> is_end(n, true);
        for (int turn = 0; ; turn++) {
            queue<int> q;
            vector<int> used(n);
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < g[i].size(); j++) {
                    int v = g[i][j];
                    if (ans1[v] == -1) {
                        used[i]++;
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
            vector<bool> check(n);
            while (!q.empty()) {
                int v = q.front();
                q.pop();
                ans1[v] = turn;
                for (int j = 0; j < gr[v].size(); j++) {
                    int u = gr[v][j];
                    if ((ans1[u] == -1) && (!check[u])) {
                        check[u] = true;
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
            }
            for (int i = 0; i < n; i++) {
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
        ans[ii] = vector<pair<int, set<int>>>(n);
        for (int i = 0; i < n; i++) {
            ans[ii][i].first = ans1[i];
            if (ans1[i] == -1) {
                for (int j = 0; j < g[i].size(); j++) {
                    int v = g[i][j];
                    if (ans1[v] != -1) ans[ii][i].second.insert(ans1[v]);
                }
            }
        }
    }
    int t;
    cin >> t;
    for (int i = 0; i < t; i++) {
        int v1, v2;
        cin >> v1 >> v2;
        --v1, --v2;
        if ((ans[0][v1].first == -1) && (ans[1][v2].first == -1)) {
            cout << "draw\n";
        }
        else if ((ans[0][v1].first == -1) && (ans[1][v2].first != -1)) {
            if (ans[0][v1].second.count(ans[1][v2].first) != 0) {
                cout << "first\n";
            }
            else {
                cout << "draw\n";
            }
        }
        else if ((ans[0][v1].first != -1) && (ans[1][v2].first == -1)) {
            if (ans[1][v2].second.count(ans[0][v1].first) != 0) {
                cout << "first\n";
            }
            else {
                cout << "draw\n";
            }
        }
        else {
            if (ans[0][v1].first != ans[1][v2].first) {
                cout << "first\n";
            }
            else {
                cout << "second\n";
            }
        }
    }
}
