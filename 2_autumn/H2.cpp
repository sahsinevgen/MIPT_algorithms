#include <iostream>
#include <vector>

using namespace std;

int64_t M = 1e9 + 7;
int64_t p = 31;

int main() {
    string s, t;
    cin >> s >> t;
    vector<int64_t> h1(s.size() + 1), 
                    h2(t.size() + 1), 
                    pw(t.size() + s.size() + 1);
    pw[0] = 1;
    for (int i = 1; i <= s.size(); i++) {
        pw[i] = (pw[i - 1] * p) % M;
    }
    h1[0] = 0;
    for (int i = 1; i <= s.size(); i++) {
        h1[i] = (h1[i - 1] + pw[i - 1] * (s[i - 1] - 'a' + 1) + M) % M;
    }
    h2[0] = 0;
    for (int i = 1; i <= t.size(); i++) {
        h2[i] = (h2[i - 1] + pw[i - 1] * (t[i - 1] - 'a' + 1) + M) % M;
    }
    vector<int> ans;
    for (int i = 0; i < s.size() - t.size() + 1; i++) {
        if ((h1[t.size() + i] - h1[i] + M) % M == (h2[t.size()] * pw[i]) % M) {
            ans.push_back(i + 1);
        }
    }
    cout << ans.size() << endl;
    for (int i = 0; i < ans.size(); i++) {
        cout << ans[i] << " ";
    }
    cout << endl;
}
