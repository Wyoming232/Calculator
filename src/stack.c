#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include "calculator.h"

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