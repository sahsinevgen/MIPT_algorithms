#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Node {
    vector<Node*> moves;
    int term;
    int cnt;

    Node() {
        moves.resize(26, NULL);
        cnt = 0;
        term = 0;
    }

    ~Node() {
        for (int i = 0; i < moves.size(); i++) {
            if (moves[i]) {
                delete moves[i];
            }
        }
    }
     
    Node*& operator[] (int x) {
        if (!moves[x]) {
            moves[x] = new Node;
        }
        return moves[x];
    }

    Node*& operator[] (char c) {
        if (!moves[c - 'a']) {
            moves[c - 'a'] = new Node;
        }
        return moves[c - 'a'];
    }

    void print() {
        for (int i = 0; i < 26; i++) {
            cout << char('a' + i);
        }
        cout << endl;
        for (int i = 0; i < 26; i++) {
            cout << (moves[i] ? '+' : '-');
        }
        cout << endl << term << endl << cnt << endl << endl;
        for (int i = 0; i < 26; i++) {
            if (moves[i]) {
                moves[i]->print();
            }
        }
    }
};

int cnt(Node *node) {
    return (node ? node->cnt : 0);
}

int cnt_child(Node *node, char i) {
    return (node->moves[i] ? node->moves[i]->cnt : 0);
}

int main() {
    Node* root = new Node;
    int n;
    cin >> n;
    while (n--) {
        //root->print();
        string s;
        cin >> s;
        if ('0' <= s[0] && s[0] <= '9') {
            //cout << "GOGOGOOGOGOGOOGOGOOGOGOGOGOGGO" << endl;
            int k = stoi(s);
            Node *cur = root;
            string ans = "";
            while (1) {
                int i;
                if (k <= cur->term) {
                    cout << ans << endl;
                    break;
                } else {
                    k -= cur->term;
                }
                for (i = 0; i < 27; i++) {
                    if (cnt_child(cur, i) < k) {
                        k -= cnt_child(cur, i);
                    } else {
                        break;
                    }
                } 
                ans += ('a' + i);
                cur = (*cur)[i];
            }
        } else {
            Node *cur = root;
            for (int i = 0; i < s.size(); i++) {
                cur = (*cur)[s[i]];
                cur->cnt++;
            }
            cur->term++;
        }
    }
}
