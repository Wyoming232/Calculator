typedef struct{
    char * collection;
    int capacity;
    int size;
}Stack;

typedef enum {
    PREC_NONE = 0,    // For non-operators or initial state
    PREC_TERM = 1,        // + -
    PREC_FACTOR = 2,      // * /
    PREC_CALL = 3,        // ()
    PREC_PRIMARY = 4      // Literals, group ()
} Precedence;

#define MAX_TOKENS 256
#define EXIT "exit"

float calculator(char * seq);
void infix_postfix(Stack * stack, char * tok, char * arr);
bool validityCheck(Stack * stack);

// Getting precedence of an operator, and highest precedence in the stack respectively
Precedence getPrecedence(char operator);
Precedence getMaxPrecedence(Stack * stack);

// Stack Specific functions
Stack * createStack(int);
void destroyStack(Stack *);
bool isFull(Stack *);
bool isEmpty(Stack *);
bool peek(Stack *, char *);
bool pop(Stack *, char *);
bool push(Stack *, char);