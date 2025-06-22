#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include <ctype.h>
#include "calculator.h"

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
        case '(':
            return PREC_OPEN;
        case ')':
            return PREC_CLOSE;
        default:
            return PREC_NONE;
    }
}

/* Precedence getMaxPrecedence(Stack * stack) {
    if (isEmpty(stack)) {
        return PREC_NONE; // No operators in stack
    }

    Precedence maxPrecedence = PREC_NONE;
    for (int i = 0; i < stack->size; i++) {
        char * operator = stack->collection[i];
        Precedence currentPrecedence = getPrecedence(operator);
        if (currentPrecedence > maxPrecedence) {
            maxPrecedence = currentPrecedence;
        }
    }
    return maxPrecedence;
} */