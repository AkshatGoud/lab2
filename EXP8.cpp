#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_PROD 20
#define MAX_ITEMS 100
#define MAX_STATES 50

typedef struct {
    char lhs;
    char rhs[20];
} Production;

typedef struct {
    int prod_index;
    int dot_pos;
} Item;

typedef struct {
    Item items[MAX_ITEMS];
    int count;
} State;

Production prods[MAX_PROD];
int n; // number of productions

State states[MAX_STATES];
int state_count = 0;

// Check if two items are equal
int equal_item(Item a, Item b) {
    return a.prod_index == b.prod_index && a.dot_pos == b.dot_pos;
}

// Check if two states are equal
int equal_state(State *a, State *b) {
    if (a->count != b->count)
        return 0;
    for (int i = 0; i < a->count; i++) {
        int found = 0;
        for (int j = 0; j < b->count; j++) {
            if (equal_item(a->items[i], b->items[j])) {
                found = 1;
                break;
            }
        }
        if (!found) return 0;
    }
    return 1;
}

// Add item to a state if not already present
void add_item(State *s, Item item) {
    for (int i = 0; i < s->count; i++) {
        if (equal_item(s->items[i], item))
            return;
    }
    s->items[s->count++] = item;
}

// Compute closure of a state
void closure(State *s) {
    int i = 0;
    while (i < s->count) {
        Item item = s->items[i];
        char *rhs = prods[item.prod_index].rhs;
        if (item.dot_pos < strlen(rhs)) {
            char sym = rhs[item.dot_pos];
            if (sym >= 'A' && sym <= 'Z') {
                for (int j = 0; j < n; j++) {
                    if (prods[j].lhs == sym) {
                        Item new_item = {j, 0};
                        add_item(s, new_item);
                    }
                }
            }
        }
        i++;
    }
}

// GOTO function: builds a new state from current state and symbol
State goto_fn(State *s, char symbol) {
    State new_state = { .count = 0 };
    for (int i = 0; i < s->count; i++) {
        Item item = s->items[i];
        char *rhs = prods[item.prod_index].rhs;
        if (item.dot_pos < strlen(rhs) && rhs[item.dot_pos] == symbol) {
            Item moved = { item.prod_index, item.dot_pos + 1 };
            add_item(&new_state, moved);
        }
    }
    closure(&new_state);
    return new_state;
}

// Add new state if not already present
int add_state(State *s) {
    for (int i = 0; i < state_count; i++) {
        if (equal_state(&states[i], s))
            return i;
    }
    states[state_count] = *s;
    return state_count++;
}

// Display a single state
void print_state(State *s, int index) {
    printf("I%d:\n", index);
    for (int i = 0; i < s->count; i++) {
        Item item = s->items[i];
        printf("%c -> ", prods[item.prod_index].lhs);
        for (int j = 0; j < strlen(prods[item.prod_index].rhs); j++) {
            if (j == item.dot_pos)
                printf(".");
            printf("%c", prods[item.prod_index].rhs[j]);
        }
        if (item.dot_pos == strlen(prods[item.prod_index].rhs))
            printf(".");
        printf("\n");
    }
    printf("\n");
}

int main() {
    printf("Enter number of productions (including augmented): ");
    scanf("%d", &n);

    printf("Enter productions (format A->BCD):\n");

    /* Example Input:
       4
       S'->S
       S->CC
       C->cC
       C->d
       (Make sure to type without spaces, exactly as shown above)
    */

    for (int i = 0; i < n; i++) {
        char buffer[30];
        scanf("%s", buffer);
        prods[i].lhs = buffer[0];
        strcpy(prods[i].rhs, buffer + 3);
    }

    State start = { .count = 0 };
    Item start_item = {0, 0}; // Assume augmented production is first
    add_item(&start, start_item);
    closure(&start);
    add_state(&start);

    for (int i = 0; i < state_count; i++) {
        print_state(&states[i], i);

        // Get all symbols from items in this state
        char symbols[50];
        int sym_count = 0;

        for (int j = 0; j < states[i].count; j++) {
            Item item = states[i].items[j];
            char *rhs = prods[item.prod_index].rhs;
            if (item.dot_pos < strlen(rhs)) {
                char sym = rhs[item.dot_pos];
                int exists = 0;
                for (int k = 0; k < sym_count; k++) {
                    if (symbols[k] == sym) {
                        exists = 1;
                        break;
                    }
                }
                if (!exists) symbols[sym_count++] = sym;
            }
        }

        for (int j = 0; j < sym_count; j++) {
            State next = goto_fn(&states[i], symbols[j]);
            int idx = add_state(&next);
            printf("GOTO(I%d, %c) = I%d\n", i, symbols[j], idx);
        }
        printf("\n");
    }

    return 0;
}