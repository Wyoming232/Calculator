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
    char * arr = malloc(sizeof(char) * len);

    Stack * stack = createStack(len);

    infix_postfix(stack, token, arr);
}

// This function gets the precedence of an operator
Precedence getPrecedence(char operator) {
    switch (operator) {
        case '+':
        case '-':
            return PREC_TERM;
        case '*':
        case '/':
            return PREC_FACTOR;
        case '(':
            return PREC_CALL;
        case ')':
            return PREC_PRIMARY;
        default:
            return PREC_NONE;
    }
}

Precedence getMaxPrecedence(Stack * stack) {

}
// This function employs the shunting yard algorithm to determine the postfix expression from the infix
// Shunting yard alg: (1) Employs a stack to store operators and an array to store numbers
//                    (2) If maxPrec is PREC_CALL, and nextPrec is PREC_PRIMARY, pop all operators between matched parentheses
//                    off the stack and put them onto arr
//                    (3) When an operator of lower precedence is to be put on top of another with higher precedence, the operator with higher
//                    precedence is popped of the stack and stored in the next slot of the array
//                    (4) The final postfix expression is stored in the decimal array
void infix_postfix(Stack * stack, char * tok, char * arr) {
    // stores the precedence of the operator on top of the stack, the precedence of the next character, 
    // and the greatest precedence in the stack currently (for parentheses matching)
    Precedence currPrec = PREC_NONE;
    Precedence nextPrec;
    Precedence maxPrec = PREC_NONE;

    // next character buffer, top character buffer, general character buffer, and iterating variable
    char buffNext;
    char buffTop;
    char buff; 
    int i, j;

    while((buffNext = tok[i++]) != '\0'){
        currPrec = getPrecedence(buffTop);
        nextPrec = getPrecedence(buffNext);
        // (2)
        if(maxPrec == PREC_CALL && nextPrec == PREC_PRIMARY) {
            while(buffTop != PREC_CALL) {
                pop(stack, buff);
                arr[j++] = buff;
            }
            // increments i so that buffNext is the token after closing parentheses
            // and pops '(' off the stack to disclude it from postfix expression
            pop(stack, buff);
            maxPrec = getMaxPrecedence(stack);
            i++;
        }
        // (3)
    }
}
// takes array of postfix expression from calculator() function and detemines validity
// Case 1 : division by 0
// Case 2 : check whether the postfix stack has an odd number of elements (requirement for valid stack). This is just a prelim check, since it is possible that there are two unpaired operators or operands and several other disqualifying cases.
// Case 3 : Incorrect ordering of parentheses (expression starts with ')' rather than '(', which equates to seeing ')' on the stack
bool validityCheck(Stack * stack) {

}