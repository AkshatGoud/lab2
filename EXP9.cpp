#include <iostream>
#include <stack>
#include <algorithm>
#include <cctype>

using namespace std;

// Function to check precedence of operators
int precedence(char op) {
    if(op == '+' || op == '-') return 1;
    if(op == '*' || op == '/') return 2;
    return 0;
}

// Check if a character is an operator
bool isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

// Convert infix expression to postfix
string infixToPostfix(const string& infix) {
    stack<char> st;
    string postfix;

    for(char c : infix) {
        if(isalnum(c)) {
            // If operand, add directly to result
            postfix += c;
        } else if(c == '(') {
            st.push(c);
        } else if(c == ')') {
            // Pop till matching '('
            while(!st.empty() && st.top() != '(') {
                postfix += st.top();
                st.pop();
            }
            st.pop(); // remove '('
        } else {
            // Operator: pop all higher or equal precedence operators
            while(!st.empty() && precedence(st.top()) >= precedence(c)) {
                postfix += st.top();
                st.pop();
            }
            st.push(c);
        }
    }

    // Pop remaining operators
    while(!st.empty()) {
        postfix += st.top();
        st.pop();
    }

    return postfix;
}

// Convert infix expression to prefix
string infixToPrefix(string infix) {
    // Reverse the expression and swap parentheses
    reverse(infix.begin(), infix.end());

    for(char& c : infix) {
        if(c == '(') c = ')';
        else if(c == ')') c = '(';
    }

    // Convert to postfix and then reverse
    string postfix = infixToPostfix(infix);
    reverse(postfix.begin(), postfix.end());
    return postfix;
}

int main() {
    string infix;

    // 📌 Sample Input (when prompted):
    // a+b*(c-d)

    cout << "Enter an infix expression (e.g., a+b*(c-d)): ";
    cin >> infix;

    string postfix = infixToPostfix(infix);
    string prefix = infixToPrefix(infix);

    // 💡 Output will show intermediate code representations
    cout << "Intermediate Code (Postfix): " << postfix << endl;
    cout << "Intermediate Code (Prefix): " << prefix << endl;

    return 0;
}