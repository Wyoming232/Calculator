#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<math.h>
#include <ctype.h>
#include "calculator.h"

// Utility functions
bool isDigit(char c) {
    return (c >= '0' && c <= '9');
}

bool isOperatorOrParenthesis(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^' || c == '(' || c == ')');
}

bool isOperator(char *s) {
    return ((s != NULL) && ((strcmp(s, "+") == 0 || strcmp(s, "-") == 0 ||
            strcmp(s, "*") == 0 || strcmp(s, "/") == 0) || strcmp(s, "^") == 0 || strcmp(s, "%") == 0));
}

bool isLeftAssociative(char * operator) {
    // Check if the operator is left associative
    return (strcmp(operator, "+") == 0 || strcmp(operator, "-") == 0 ||
            strcmp(operator, "*") == 0 || strcmp(operator, "/") == 0 || strcmp(operator, "%") == 0);
}

char * substring (char * str, int start, int end) {
    if (str == NULL || start < 0 || end < start || end >= strlen(str)) {
        fprintf(stderr, "Error: Invalid substring parameters\n");
        return NULL; // Handle error
    }
    
    char * sub = (char *)malloc((end - start + 2) * sizeof(char)); // +1 for null terminator
    if (sub == NULL) {
        perror("Memory allocation failed for substring");
        return NULL; // Handle memory allocation failure
    }

    sub = strncpy(sub, str + start, end - start + 1);
    sub[end - start + 1] = '\0'; // Null-terminate the substring

    return sub;
}

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
        case '^': 
            result_str = numToString((int)pow(c, d)); 
            break; // Using pow for exponentiation
        case '%':
            result_str = numToString(c % d);
            break;
        default:
            fprintf(stderr, "Error: Unknown operator '%s'\n", op);
            return NULL; // Indicate error
    }
    return result_str;
}

// This function tokenizes the input string by operator and stores each token in the tokens array (can be modified later to tokenize by other delimiters
// if needed)
void tokenize(char * seq, char ** tokens, int  * num_tokens) {
    int size = strlen(seq);
    int subStartIdx = 0;
    int subEndIdx = 0;

    for (int i = 0; i < size; i++) {
        if (seq[i] == ' ') {
            continue; // Skip spaces
        } else if (isDigit(seq[i])) {
            subStartIdx = i; // Start of a number
            while (i < size && isDigit(seq[i])) {
                i++; // Move to the end of the number
            }
            subEndIdx = i - 1; // End of the number
            tokens[*num_tokens] = substring(seq, subStartIdx, subEndIdx);
            *num_tokens += 1; // Increment the number of tokens
            i--; // Adjust i since the for loop will increment it
        } else if(isOperatorOrParenthesis(seq[i])) {
            // Handle operators
            tokens[*num_tokens] = substring(seq, i, i);
            *num_tokens += 1; // Increment the number of tokens
        }
    }

    tokens[*num_tokens] = NULL; // 4. Null-terminates the array of pointers
}

// This function gets the precedence of an operator
Precedence getPrecedence(char * operator) {
    if (operator == NULL || strlen(operator) != 1) {
        return PREC_NONE; // Invalid operator
    }
    switch (operator[0]) {
        case '+':
        case '-':
        case '%':
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