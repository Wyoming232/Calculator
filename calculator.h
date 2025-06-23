#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include <ctype.h>

typedef struct{
    char ** collection;
    int capacity;
    int size;
}Stack;

typedef enum {
    PREC_NONE = 0,    // For parens or initial state
    PREC_ADDSUB = 1,        // + -
    PREC_MULDIV = 2,      // * /
    PREC_EXP = 3,    // ^ (exponentiation)
} Precedence;

#define MAX_TOKENS 256
#define EXIT "exit"

void calculator(char * seq);
char ** infix_postfix(Stack * stack, char ** tok, char ** arr, int num_tokens);
void tokenize(char * seq, char ** tokens, int * num_tokens);
bool validityCheck(Stack * stack);

// Getting precedence of an operator, and highest precedence in the stack respectively
Precedence getPrecedence(char * operator);
Precedence getMaxPrecedence(Stack * stack);

// Stack Specific functions
Stack * createStack(int);
void destroyStack(Stack *);
bool isFull(Stack *);
bool isEmpty(Stack *);
char * peek(Stack *);
bool pop(Stack *, char **);
bool push(Stack *, char *);