#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<math.h>
#include <ctype.h>
#include "calculator.h"

// Utility functions
int stringToNum(char * a) {
    if (a == NULL) {
        fprintf(stderr, "Error: NULL string passed to stringToNum\n");
        return 0; // Or a specific error handling
    }
    // Using strtol for robustness
    char *endptr;
    long val = strtol(a, &endptr, 10);

    if (*endptr != '\0') {
        fprintf(stderr, "Error: Invalid character in number string '%s'\n", a);
        // Depending on strictness, you might want to return an error or throw
        return 0;
    }
    // Basic range check, assuming int fits long
    if (val > 2147483647 || val < -2147483648) { // INT_MAX and INT_MIN
        fprintf(stderr, "Error: Number '%s' out of int range\n", a);
        // Handle overflow/underflow if necessary
    }
    return (int)val;
}

void reverse(char *str) {
    int length = strlen(str);
    int i, j;
    char temp;
    for (i = 0, j = length - 1; i < j; i++, j--) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}

char *numToString(int num) {
    char *str = NULL;
    int isNegative = 0;
    long long tempNum;

    if (num == 0) {
        str = (char *)malloc(2 * sizeof(char));
        if (str == NULL) { perror("Memory allocation failed"); return NULL; }
        str[0] = '0';
        str[1] = '\0';
        return str;
    }

    if (num < 0) {
        isNegative = 1;
        tempNum = (long long)num * -1;
    } else {
        tempNum = num;
    }

    int numDigits = 0;
    long long checkNum = tempNum;
    do {
        checkNum /= 10;
        numDigits++;
    } while (checkNum > 0);

    str = (char *)malloc((numDigits + isNegative + 1) * sizeof(char));
    if (str == NULL) { perror("Memory allocation failed"); return NULL; }

    int current_idx = 0;
    if (isNegative) {
        str[current_idx++] = '-';
    }

    int digit_start_idx = current_idx;
    do {
        str[current_idx++] = (char)(tempNum % 10 + '0');
        tempNum /= 10;
    } while (tempNum > 0);

    str[current_idx] = '\0';

    // Reverse only the digit part
    if (isNegative) {
        reverse(str + digit_start_idx);
    } else {
        reverse(str);
    }

    return str;
}

char * performOperation(char * a, char * b, char * op) {
    int c = stringToNum(a);
    int d = stringToNum(b);

    char *result_str = NULL;

    switch (op[0]) {
        case '+': result_str = numToString(c + d); break;
        case '-': result_str = numToString(c - d); break;
        case '*': result_str = numToString(c * d); break;
        case '/':
            if (d == 0) {
                fprintf(stderr, "Error: Division by zero (%d / %d)\n", c, d);
                return NULL; // Indicate error
            }
            result_str = numToString(c / d);
            break;
        case '^': result_str = numToString((int)pow(c, d)); break; // Using pow for exponentiation
        default:
            fprintf(stderr, "Error: Unknown operator '%s'\n", op);
            return NULL; // Indicate error
    }
    return result_str;
}

bool isOperator(char *s) {
    return ((s != NULL) && ((strcmp(s, "+") == 0 || strcmp(s, "-") == 0 ||
            strcmp(s, "*") == 0 || strcmp(s, "/") == 0) || strcmp(s, "^") == 0));
}

bool isLeftAssociative(char * operator) {
    // Check if the operator is left associative
    return (strcmp(operator, "+") == 0 || strcmp(operator, "-") == 0 ||
            strcmp(operator, "*") == 0 || strcmp(operator, "/") == 0);
}

// This function tokenizes the input string by spaces and stores each token in the tokens array (can be modified later to tokenize by other delimiters
// if needed)
void tokenize(char * seq, char ** tokens, int  * num_tokens) {
    char *str_copy = strdup(seq); // 1. Makes a copy to protect original 'seq'
    if (str_copy == NULL) {
        perror("Failed to copy string for tokenization");
        tokens[0] = NULL;
        return;
    }

    char *token;
    char *rest = str_copy; // Used by strtok_r to maintain internal state
    int i = 0;

    // 2. Correct loop condition: checks return value of strtok_r and bounds
    while ((token = strtok_r(rest, " ", &rest)) != NULL && i < MAX_TOKENS) {
        tokens[i] = strdup(token); // 3. Copies each token into new memory
        (*num_tokens)++; // Increment the count of tokens
        if (tokens[i] == NULL) {
            perror("Failed to duplicate token");
            // --- Crucial cleanup if duplication fails mid-loop ---
            for (int j = 0; j < i; j++) {
                free(tokens[j]);
            }
            free(str_copy);
            tokens[0] = NULL;
            return;
        }
        i++;
    }
    tokens[i] = NULL; // 4. Null-terminates the array of pointers
    free(str_copy); // 5. Frees the temporary copy
}

// This function gets the precedence of an operator
Precedence getPrecedence(char * operator) {
    if (operator == NULL || strlen(operator) != 1) {
        return PREC_NONE; // Invalid operator
    }
    switch (operator[0]) {
        case '+':
        case '-':
            return PREC_ADDSUB;
        case '*':
        case '/':
            return PREC_MULDIV;
        case '^':
            return PREC_EXP; // Assuming '^' has the highest precedence
        default:
            return PREC_NONE;
    }
}

// Stack Functions
Stack * createStack(int cap) {
    if(cap <= 0) return NULL;

    Stack * stack = malloc(sizeof(Stack));
    if(stack == NULL) {
        perror("Stack allocation failed"); // Use perror for system errors
        return NULL;
    }

    stack->collection = (char**)calloc(cap, sizeof(char*));
    if(stack->collection == NULL) {
        perror("Collection allocation failed");
        free(stack);
        return NULL;
    }

    stack->capacity = cap;
    stack->size = 0;

    return stack;
}

void destroyStack(Stack * stack) {
    if (stack == NULL) return;

    for (int i = 0; i < stack->capacity; i++) {
        free(stack->collection[i]);
    }

    free(stack->collection);
    free(stack);
}

bool isFull(Stack * stack) {
    return stack->size == stack->capacity;
}

bool isEmpty(Stack * stack) {
    return (stack->size == 0);
}

// Pop an element. Caller takes ownership of the char* and must free it.
bool pop(Stack * stack, char ** top) {
    if (isEmpty(stack)) return false;

    stack->size--;
    *top = stack->collection[stack->size]; // Get the element

    stack->collection[stack->size] = NULL; // Clear the slot for safety
    // Note: Caller is responsible for freeing (1) the string pointed to by *top and (2)stack->collection[stack->size] since it was dynamically allocated.

    return true;
}

// Push an element. Creates a copy of the string.
bool push(Stack * stack, char * str_to_push) {
    if(isFull(stack)) return false;

    // Allocate memory for the new string
    char *new_str = strdup(str_to_push);

    stack->collection[stack->size] = new_str; // Store the pointer to the new string
    stack->size++;
    return true;
}

// Peek an element (no memory management implications)
char * peek(Stack * stack) {
    if (isEmpty(stack)) return NULL;

    return stack->collection[stack->size - 1];
}