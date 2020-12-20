#include <iostream>
#include <vector>

using namespace std;

int main() {
    string s;
    cin >> s;
    int n = s.size();
    vector<int> z(n);
    z[0] = n;
    int l = 0;
    int r = 0;
    for (int i = 1; i < n; i++) {
        z[i] = max(0, min(r - i, z[i - l]));
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
            z[i]++;
        }
        if (i + z[i] > r) {
            l = i;
            r = i + z[i];
        }
    }
    for (int i = 0; i < n; i++) {
        cout << z[i] << " ";
    }
    cout << endl;
}
