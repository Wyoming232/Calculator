#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include <ctype.h>
#include "calculator.h"

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