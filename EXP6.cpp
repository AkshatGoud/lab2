#include <stdio.h>
#include <string.h>

#define MAX 100

char stack[MAX][MAX];
char input[MAX][MAX];
int top = -1;
int ip = 0;

// Push a symbol to the stack
void push(char *symbol) {
    strcpy(stack[++top], symbol);
}

// Pop the top of the stack
void pop() {
    if (top >= 0) top--;
}

// Display stack and input for tracing
void display() {
    printf("Stack: ");
    for (int i = 0; i <= top; i++)
        printf("%s ", stack[i]);
    printf("\tInput: ");
    for (int i = ip; input[i][0] != '\0'; i++)
        printf("%s ", input[i]);
    printf("\n");
}

// Try to reduce based on grammar rules
int reduce() {
    // E -> id
    if (strcmp(stack[top], "id") == 0) {
        pop();
        push("E");
        printf("Reduced: E -> id\n");
        return 1;
    }

    // E -> ( E )
    if (top >= 2 && strcmp(stack[top - 2], "(") == 0 &&
        strcmp(stack[top - 1], "E") == 0 &&
        strcmp(stack[top], ")") == 0) {
        pop(); pop(); pop();
        push("E");
        printf("Reduced: E -> ( E )\n");
        return 1;
    }

    // E -> E + E
    if (top >= 2 && strcmp(stack[top - 2], "E") == 0 &&
        strcmp(stack[top - 1], "+") == 0 &&
        strcmp(stack[top], "E") == 0) {
        pop(); pop(); pop();
        push("E");
        printf("Reduced: E -> E + E\n");
        return 1;
    }

    // E -> E * E
    if (top >= 2 && strcmp(stack[top - 2], "E") == 0 &&
        strcmp(stack[top - 1], "*") == 0 &&
        strcmp(stack[top], "E") == 0) {
        pop(); pop(); pop();
        push("E");
        printf("Reduced: E -> E * E\n");
        return 1;
    }

    return 0; // No reduction done
}

int main() {
    char str[MAX];

    // Sample Input:
    // id + id * id
    // This corresponds to the expression: id + (id * id)
    // which is valid for grammar:
    // E -> id | (E) | E + E | E * E

    printf("Enter the input string (tokens separated by space, e.g., id + id * id):\n");
    fgets(str, MAX, stdin);

    // Tokenize input
    char *token = strtok(str, " \n");
    int i = 0;
    while (token != NULL) {
        strcpy(input[i++], token);
        token = strtok(NULL, " \n");
    }
    strcpy(input[i], "\0");

    printf("\nShift Reduce Parsing Steps:\n");

    // Parsing loop
    while (1) {
        // Shift
        if (input[ip][0] != '\0') {
            printf("\nAction: SHIFT %s\n", input[ip]);
            push(input[ip]);
            ip++;
            display();

            // Try to reduce as much as possible
            while (reduce()) {
                display();
            }
        } else {
            // No more input, try final reductions
            if (!reduce())
                break;
            display();
        }
    }

    // Final acceptance check
    if (top == 0 && strcmp(stack[0], "E") == 0) {
        printf("\nInput accepted!\n");
    } else {
        printf("\nInput rejected.\n");
    }

    return 0;
}