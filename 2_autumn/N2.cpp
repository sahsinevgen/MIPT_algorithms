#include <iostream>
#include <vector>

using namespace std;

int main() {
    int n;
    cin >> n;
    vector<int> z(n), p(n);
    for (int i = 0; i < n; i++) {
        cin >> p[i];
    }
    for (int i = 1; i < n; i++) {
        if (p[i] > 0) {
            z[i - p[i] + 1] = p[i];
        }
    }
    z[0] = n;
    for (int i = 1; i < n; ) {
        int i_ = i;
        if (z[i] > 0) {
            for (int j = 1; j < z[i]; j++) {
                if (z[i + j] > z[j]) {
                    break;
                }
                z[i + j] = min(z[j], z[i] - j);
                i_ = i + j;
            }
        }
        i = i_ + 1;
    }
    for (int i = 0; i < n; i++) {
        cout << z[i] << " ";
    }
    cout << endl;
}
