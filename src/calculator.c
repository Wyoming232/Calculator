// This file contains the the implementation of the parser that processes command line args to extract
// the mathematical expression and its components.

#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include "calculator.h"

// seq is the expression string input into the command line
// it will converted from infix to postfix using the shunting-yard algorithm (stack + array implementation)
// the postfix expression will be evaluated using a stack:
// push numbers onto stack, once you see an operator pop top two numbers, evaluate expression, and push back onto stack
float calculator(char * seq) {
    int len = strlen(seq);
    char * token = strtok(seq, " ");

    Stack * stack = createStack(len);

    infix_postfix(stack, token);
}

void infix_postfix(Stack * stack, char * arr) {
    
}
// takes array of postfix expression from calculator() function and detemines validity
// Case 1 : division by 0
// Case 2 : unpaired operator (includes parentheses) or operand
bool validityCheck(Stack * stack) {

}

Stack * createStack(int cap) {
    if(cap <= 0) return NULL;

    Stack * stack = malloc(sizeof(Stack));

    if(stack == NULL) {
        printf("Stack allocation failed");
        return NULL;
    }

    stack->collection = malloc(sizeof(char) * cap);

    if(stack->collection == NULL) {
        printf("Collection allocation failed");
        free(stack);
        return NULL;
    }

    stack->capacity = cap;
    stack->size = 0;

    return stack;
}

void destroyStack(Stack * stack) {
    free(stack->collection);
    free(stack);
}

bool isFull(Stack * stack) {
    if(stack->capacity == stack->size) return true;

    return false;
}

bool isEmpty(Stack * stack) {
    if(stack->size == 0) return true;

    return false;
}

bool pop(Stack * stack, char * top) {
    if(isEmpty(stack)) return false;

    *top = *(stack->collection + stack->size - 1);
    *(stack->collection + stack->size - 1) = 0;
    stack->size--;
    
    return true;
}

bool push(Stack * stack, char top) {
    if(isFull(stack)) return false;

    *(stack->collection + stack->size) = top;
    stack->size++;

    return true;
}

bool peek(Stack * stack, char * top) {
    if(isEmpty(stack)) return false;

    *top = (stack->collection[stack->size - 1]);
    return true;
}