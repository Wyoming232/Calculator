typedef struct{
    char * collection;
    int capacity;
    int size;
}Stack;

#define MAX_TOKENS 256
#define EXIT "exit"

float calculator(char * seq);
//Stack Specific functions
Stack * createStack(int);
void destroyStack(Stack *);
bool isFull(Stack *);
bool isEmpty(Stack *);
bool peek(Stack *, char *);
bool pop(Stack *, char *);
bool push(Stack *, char);