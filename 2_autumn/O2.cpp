#include <iostream>
#include <vector>

using namespace std;

int main() {
    string s;
    cin >> s;
    int n = s.size();
    vector<int> p(n);
    p[0] = 0;
    for (int i = 1; i < n; i++) {
        p[i] = p[i - 1];
        while (p[i] > 0 && s[i] != s[p[i]]) {
            p[i] = p[p[i] - 1];
        }
        if (s[i] == s[p[i]]) {
            p[i]++;
        }
    }
    vector<int> f(n);
    f[0] = 0;
    for (int i = 0; i < n; i++) {
        if (p[i] > 0) {
            f[i] = f[p[i] - 1] + 1;
        } else {
            f[i] = 0;
        }
    }
    int id = 0;
    for (int i = 0; i < n; i++) {
        if (f[i] > f[id]) {
            id = i;
        }
    }
    cout << s.substr(0, id + 1) << endl;
}
