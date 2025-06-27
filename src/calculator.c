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
int calculator(char * seq) {
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

    arr = infix_postfix(stack, tokens, arr, &num_tokens);

    destroyStack(stack); // Clean up the stack to prevent memory leaks

    return evalRPN(arr, num_tokens); // Evaluate the postfix expression and return the result
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
char ** infix_postfix(Stack * stack, char ** tokens, char ** arr, int * num_tokens) {
    int num_arr = *num_tokens;
    char * curr, * next, *popped;
    Precedence currPrecedence = PREC_NONE, nextPrecedence = PREC_NONE;
    int curr_pos_tokens = 0;
    int arr_pos = 0;

    while (curr_pos_tokens < num_arr) {
        curr = peek(stack); // Get the top of the stack
        next = tokens[curr_pos_tokens]; // Get the next token
        currPrecedence = getPrecedence(curr);
        nextPrecedence = getPrecedence(next);

        if (!nextPrecedence && strcmp(next, "(") && strcmp(next, ")")){
            arr[arr_pos++] = strdup(next); // Case 1: next is a number or variable, store it in the array
        } else if (strcmp(next, "(") == 0) {
            // Special case: if the current precedence is '(', we need to handle it differently
            push(stack, next); // If the current precedence is '(', push the next token onto the stack
        } else if(strcmp(next, ")") == 0) {
            // Case 5: maxPrecedence is 1 and nextPrecedence is ')'
            popped = peek(stack); // Peek the top of the stack
            while(strcmp(popped, "(") != 0) {
                pop(stack, &popped); // Pop all operators off the stack until '('
                if(strcmp(popped, "(") == 0) {
                    break;
                }
                arr[arr_pos++] = popped; // Store operators in the array
            }
            *num_tokens -= 2; // Decrease the number of tokens since we popped a parenthesis
        } else if(isEmpty(stack)) {
            push(stack, next); // Case 2: next is an operator and stack is empty, push it onto the stack
        } else {
            while(!isEmpty(stack) && strcmp(curr, "(") != 0) {
                curr = peek(stack); // Get the current operator from the stack
                currPrecedence = getPrecedence(curr);
                // Case 2 & 3: next is an operator and stack is empty or nextPrecedence is greater than currPrecedence
                if ((isLeftAssociative(next) && nextPrecedence <= currPrecedence) ||
                    (!isLeftAssociative(next) && nextPrecedence < currPrecedence)) {
                    pop(stack, &popped); // Pop the current operator from the stack
                    arr[arr_pos++] = popped; // Store it in the array
                } else {
                    break;
                }
            }
            push(stack, next); // Push the next operator onto the stack
        }
        curr_pos_tokens++;
    }

    // After processing all tokens, pop any remaining operators from the stack
    while (!isEmpty(stack)) {
        pop(stack, &popped); // Pop remaining operators
        arr[arr_pos++] = popped; // Store them in the array
    }

    arr[arr_pos] = NULL; // Null-terminate the array to indicate the end of the postfix expression

    return arr; // Return the array containing the postfix expression
}

int evalRPN(char** tokens, int tokensSize) {
    Stack * stack = createStack(tokensSize); // Max capacity is tokensSize
    if (stack == NULL) {
        return 0; // Indicate error
    }

    for (int i = 0; i < tokensSize; i++) {
        char *token = tokens[i];

        if (isOperator(token)) {
            char *operand2_str = NULL;
            char *operand1_str = NULL;

            // Pop operand2 first (top of stack)
            if (!pop(stack, &operand2_str)) {
                fprintf(stderr, "Error: Not enough operands for operator '%s' (operand2 missing)\n", token);
                destroyStack(stack);
                return 0; // Indicate error
            }
            // Pop operand1 next
            if (!pop(stack, &operand1_str)) {
                fprintf(stderr, "Error: Not enough operands for operator '%s' (operand1 missing)\n", token);
                free(operand2_str); // Free the one we did pop
                destroyStack(stack);
                return 0; // Indicate error
            }

            char *result_str = performOperation(operand1_str, operand2_str, token);

            // Free the operands that were popped off the stack
            free(operand1_str);
            free(operand2_str);

            if (result_str == NULL) {
                // Error message already printed by performOperation
                destroyStack(stack);
                return 0; // Indicate error
            }

            // Push the result onto the stack
            if (!push(stack, result_str)) { // push creates a copy
                fprintf(stderr, "Error: Stack full during push of result\n");
                free(result_str); // Free the string if push fails
                destroyStack(stack);
                return 0; // Indicate error
            }
            free(result_str); // Free the original result_str generated by numToString, as push copied it

        } else { // It's an operand (number)
            if (!push(stack, token)) { // push will create a copy of token
                fprintf(stderr, "Error: Stack full during push of operand '%s'\n", token);
                destroyStack(stack);
                return 0; // Indicate error
            }
        }
    }

    // After processing all tokens, there should be exactly one item left
    char *final_result_str = NULL;
    if (stack->size != 1 || !pop(stack, &final_result_str)) {
        fprintf(stderr, "Error: Invalid RPN expression (too many/too few operands/operators left on stack)\n");
        destroyStack(stack);
        if (final_result_str) free(final_result_str); // In case of partial pop
        return 0; // Indicate error
    }

    int final_result = stringToNum(final_result_str);
    free(final_result_str); // Free the final result string

    destroyStack(stack); // Clean up the stack structure itself

    return final_result;
}