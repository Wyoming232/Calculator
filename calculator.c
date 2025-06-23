// This file contains the the implementation of the parser that processes command line args to extract
// the mathematical expression and its components.
#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include <ctype.h>
#include "calculator.h"

// seq is the expression string input into the command line
// it will converted from infix to postfix using the shunting-yard algorithm (stack + array implementation)
// the postfix expression will be evaluated using a stack:
// push numbers onto stack, once you see an operator pop top two numbers, evaluate expression, and push back onto stack
void calculator(char * seq) {
    int len = strlen(seq);
    // ensure that the input is not empty
    if(len == 0) {
        printf("Empty input. Please enter a valid expression.\n");
        // return 0.0f;
    }

    int num_tokens = 0;
    char ** tokens = malloc(sizeof(char *) * MAX_TOKENS);
    tokenize(seq, tokens, &num_tokens);
    
    char ** arr = malloc(sizeof(char *) * (num_tokens + 1));

    Stack * stack = createStack(num_tokens);

    arr = infix_postfix(stack, tokens, arr, num_tokens);

    int i = 0;
    // printf("Postfix expression: ");
    while (arr[i] != NULL) {
        printf("%s ", arr[i]); // Print each token in the postfix expression
        free(arr[i]); // Free each token after printing
        i++;
    }

    destroyStack(stack); // Clean up the stack to prevent memory leaks
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

/* This function employs the shunting yard algorithm to determine the postfix expression from the infix
Shunting yard alg: Track current position in array, current position in tokens, stack top (curr), and next string in char ** tokens (next), currPrecedence, nextPrecedence, and maxPrecedence
(to check if there is a parentheses).
Case 1 (next isNumber) : put in first open array slot
Case 2 (next isOperator, stack is empty) : push next onto stack
Case 3 (next isOperator, currPrecedence is lower than nextPrecedence) : push next onto stack
Case 4 (next isOperator, currPrecedence is higher than nextPrecedence) : pop current and put in first open array slot
Case 5 (maxPrecedence is '(' and nextPrecedence is ')') : pop all operators off the stack and discard both parentheses, then
    recalculate maxPrecedence.

Check to see if this works with test cases in main.c
 */
char ** infix_postfix(Stack * stack, char ** tokens, char ** arr, int num_tokens) {
    char * curr, * next;
    Precedence currPrecedence = PREC_NONE, nextPrecedence = PREC_NONE;
    int curr_pos_tokens = 0;
    int arr_pos = 0;
    int inParens = 0;

    while (curr_pos_tokens < num_tokens) {
        curr = peek(stack); // Get the top of the stack
        next = tokens[curr_pos_tokens]; // Get the next token
        currPrecedence = getPrecedence(curr);
        nextPrecedence = getPrecedence(next);

        if (!nextPrecedence && strcmp(next, "(") && strcmp(next, ")")){
            arr[arr_pos++] = strdup(next); // Case 1: next is a number or variable, store it in the array
        } else if (strcmp(next, "(") == 0) {
            // Special case: if the current precedence is '(', we need to handle it differently
            inParens++;
            push(stack, next); // If the current precedence is '(', push the next token onto the stack
        } else if(strcmp(next, ")") == 0 && inParens) {
            // Case 5: maxPrecedence is 1 and nextPrecedence is ')'
            char * popped = peek(stack); // Peek the top of the stack
            while(strcmp(popped, "(") != 0) {
                pop(stack, &popped); // Pop all operators off the stack until '('
                if(strcmp(popped, "(") == 0) {
                    break;
                }
                arr[arr_pos++] = popped; // Store operators in the array
            }
            inParens--; // Decrease the parentheses count
        } else if((nextPrecedence && isEmpty(stack)) || (nextPrecedence > currPrecedence)) {
            // Case 2 & 3: next is an operator and stack is empty or nextPrecedence is greater than currPrecedence
            push(stack, next); // Push the operator onto the stack
        } else if(nextPrecedence <= currPrecedence) {
            // Case 4: next is an operator and currPrecedence is higher than nextPrecedence
            char * popped;
            pop(stack, &popped); // Pop the current operator from the stack
            push(stack, next); // Push the next operator onto the stack
            arr[arr_pos++] = popped; // Store it in the array
        }
        curr_pos_tokens++;
    }

    // After processing all tokens, pop any remaining operators from the stack
    while (!isEmpty(stack)) {
        char * popped;
        pop(stack, &popped); // Pop remaining operators
        arr[arr_pos++] = popped; // Store them in the array
    }

    arr[arr_pos] = NULL; // Null-terminate the array to indicate the end of the postfix expression

    return arr; // Return the array containing the postfix expression
}

/* // takes array of postfix expression from calculator() function and detemines validity
// Case 1 : division by 0
// Case 2 : check whether the postfix stack has an odd number of elements (requirement for valid stack). This is just a prelim check, since it is possible that there are two unpaired operators or operands and several other disqualifying cases.
// Case 3 : Incorrect ordering of parentheses (expression starts with ')' rather than '(', which equates to seeing ')' on the stack
bool validityCheck(Stack * stack) {
    if(isEmpty(stack)) {
        printf("Invalid expression: Empty stack.\n");
        return false;
    }
    // Check for division by zero
    if(top == '/' && stack->size > 0) {
        printf("Invalid expression: Division by zero.\n");
        return false;
    }

    // Check for odd number of elements in the stack
    if(stack->size % 2 == 0) {
        printf("Invalid expression: Even number of elements in the stack.\n");
        return false;
    }

    // Check for incorrect ordering of parentheses
    if(top == ')') {
        printf("Invalid expression: Unmatched closing parenthesis.\n");
        return false;
    }

    return true;
} */