#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <cctype>

using namespace std;

int main() {
    int n;
    cout << "Enter number of productions: ";
    cin >> n;

    // Read productions
    // Expect format: A=α  (no spaces), e.g. E=E+T
    map<char, vector<string>> prod;
    for (int i = 0; i < n; i++) {
        string s;
        cout << "Enter production (e.g. E=E+T): ";
        cin >> s;
        char A = s[0];
        string rhs = s.substr(2);
        prod[A].push_back(rhs);
    }

    // Initialize empty LEADING and TRAILING sets
    map<char, set<char>> leading, trailing;

    // Iteratively apply rules until no set changes
    bool changed = true;
    while (changed) {
        changed = false;
        for (auto &p : prod) {
            char A = p.first;
            for (auto &rhs : p.second) {
                // LEADING: look at first symbol of rhs
                char X = rhs.front();
                if (!isupper(X)) {
                    // terminal
                    if (leading[A].insert(X).second) changed = true;
                } else {
                    // non-terminal: union its LEADING
                    for (char c : leading[X]) {
                        if (leading[A].insert(c).second) changed = true;
                    }
                }

                // TRAILING: look at last symbol of rhs
                char Y = rhs.back();
                if (!isupper(Y)) {
                    if (trailing[A].insert(Y).second) changed = true;
                } else {
                    for (char c : trailing[Y]) {
                        if (trailing[A].insert(c).second) changed = true;
                    }
                }
            }
        }
    }

    // Print results
    cout << "\nComputed LEADING and TRAILING sets:\n";
    for (auto &p : prod) {
        char A = p.first;
        cout << "LEADING(" << A << ") = { ";
        for (char c : leading[A]) cout << c << ' ';
        cout << "}\n";

        cout << "TRAILING(" << A << ") = { ";
        for (char c : trailing[A]) cout << c << ' ';
        cout << "}\n";
    }

    return 0;
}