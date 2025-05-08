#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <stack>
#include <set>
using namespace std;

int nt = 0, t = 0;
vector<char> NT, T;
vector<string> productions;
char l[10][10], tr[10][10];
stack<char> stk;

int searchNT(char a)
{
    for (int i = 0; i < nt; i++)
        if (NT[i] == a)
            return i;
    return -1;
}

int searchT(char a)
{
    for (int i = 0; i < t; i++)
        if (T[i] == a)
            return i;
    return -1;
}

void push(char a, char b)
{
    stk.push(b);
    stk.push(a);
}

void installLeading(int a, int b)
{
    if (l[a][b] == 'f')
    {
        l[a][b] = 't';
        push(NT[a], T[b]);
    }
}

void installTrailing(int a, int b)
{
    if (tr[a][b] == 'f')
    {
        tr[a][b] = 't';
        push(NT[a], T[b]);
    }
}

int main()
{
    int n;
    cout << "Enter the number of productions: ";
    cin >> n;
    productions.resize(n);

    cout << "Enter productions one by one (e.g., E->E+T):\n";
    for (int i = 0; i < n; i++)
    {
        cin >> productions[i];
        if (searchNT(productions[i][0]) == -1)
            NT.push_back(productions[i][0]), nt++;
    }

    for (int i = 0; i < n; i++)
    {
        for (size_t j = 3; j < productions[i].length(); j++)
        {
            char symbol = productions[i][j];
            if (searchNT(symbol) == -1 && searchT(symbol) == -1)
                T.push_back(symbol), t++;
        }
    }

    // Initialize LEADING and TRAILING tables
    memset(l, 'f', sizeof(l));
    memset(tr, 'f', sizeof(tr));

    // Calculate LEADING
    for (int i = 0; i < nt; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (productions[j][0] == NT[i])
            {
                char first = productions[j][3];
                if (searchT(first) != -1)
                    installLeading(i, searchT(first));
                else
                {
                    for (size_t k = 3; k < productions[j].length(); k++)
                    {
                        if (searchNT(productions[j][k]) == -1)
                        {
                            installLeading(i, searchT(productions[j][k]));
                            break;
                        }
                    }
                }
            }
        }
    }

    while (!stk.empty())
    {
        char b = stk.top();
        stk.pop();
        char a = stk.top();
        stk.pop();
        for (int i = 0; i < n; i++)
        {
            if (productions[i][3] == b)
                installLeading(searchNT(productions[i][0]), searchT(a));
        }
    }

    cout << "\nLEADING Sets:\n";
    for (int i = 0; i < nt; i++)
    {
        cout << "LEADING(" << NT[i] << ") = { ";
        for (int j = 0; j < t; j++)
            if (l[i][j] == 't')
                cout << T[j] << " ";
        cout << "}\n";
    }

    // Clear stack
    while (!stk.empty())
        stk.pop();

    // Calculate TRAILING
    for (int i = 0; i < nt; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (productions[j][0] == NT[i])
            {
                size_t lastIdx = productions[j].length() - 1;
                char last = productions[j][lastIdx];
                if (searchT(last) != -1)
                    installTrailing(i, searchT(last));
                else
                {
                    for (int k = lastIdx; k >= 3; k--)
                    {
                        if (searchNT(productions[j][k]) == -1)
                        {
                            installTrailing(i, searchT(productions[j][k]));
                            break;
                        }
                    }
                }
            }
        }
    }

    while (!stk.empty())
    {
        char b = stk.top();
        stk.pop();
        char a = stk.top();
        stk.pop();
        for (int i = 0; i < n; i++)
        {
            if (productions[i].back() == b)
                installTrailing(searchNT(productions[i][0]), searchT(a));
        }
    }

    cout << "\nTRAILING Sets:\n";
    for (int i = 0; i < nt; i++)
    {
        cout << "TRAILING(" << NT[i] << ") = { ";
        for (int j = 0; j < t; j++)
            if (tr[i][j] == 't')
                cout << T[j] << " ";
        cout << "}\n";
    }

    return 0;
}