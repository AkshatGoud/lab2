#include <iostream>
#include <algorithm>
using namespace std;

int main() {
    string s, lhs, op1, op2;
    char op;

    // 📌 Example Input: a = b + c
    getline(cin, s);

    // Remove all spaces
    s.erase(remove(s.begin(), s.end(), ' '), s.end());

    // Find '=' position
    int eq = s.find('=');

    // Find position of operator after '='
    int opPos = s.find_first_of("+-*/", eq);

    // Extract parts of the expression
    lhs = s.substr(0, eq);                             // LHS: a
    op1 = s.substr(eq + 1, opPos - eq - 1);            // operand1: b
    op = s[opPos];                                     // operator: +
    op2 = s.substr(opPos + 1);                         // operand2: c

    // Generate assembly-style code
    cout << "LOAD R1, " << op1 << "\n";
    cout << (op == '+' ? "ADD" : op == '-' ? "SUB" : op == '*' ? "MUL" : "DIV") 
         << " R1, " << op2 << "\n";
    cout << "STORE " << lhs << ", R1\n";

    return 0;
}