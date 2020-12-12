#include <list>
#include <vector>
#include <iostream>

using namespace std;
int last_digit(list<int> array, int Mod = 10) {
    if (array.size() == 0)
        return 1;
    vector<int> anss;
    int x = array.front();
    int x_ = x;
    while (true) {
        x_ = (x * x_) % Mod;
        anss.push_back(x_);
        if (x == x_)
            break;
    }
    array.pop_front();
    return (anss[last_digit(array, anss.size())] + 1) % Mod;
    // Write your code here
}
int main() {

    while(true) {
        int n;
        cin >> n;
        list<int> ar;
        for (int i = 0; i < n; i++) {
            int a;
	    cin >> a;
	    ar.push_back(a);
	}
        cout << last_digit(ar) << endl;
    }
}
