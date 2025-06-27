#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include <ctype.h>
#include "calculator.h"

#define CALCULATOR

int main() {
    #ifdef CALCULATOR
    char buffer[MAX_TOKENS]; // Buffer for user input, assuming max token length of 10
    double result;

    printf("Welcome to Calculator. Enter an expression or type 'exit' to quit.\n");

    for (;;) {
        printf(">> ");

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }

        buffer[strcspn(buffer, "\n")] = '\0';

        if (strcmp(buffer, EXIT) == 0) {
            printf("Goodbye!\n");
            break;
        }

        result = calculator(buffer);

        printf("Result: %.5f\n", result);
    }
    #endif

    #ifdef TEST_LEXER
    char* expression = "5*(100+34)/2^3";

    printf("--- Running Lexer Test ---\n");
    printf("Input Expression: \"%s\"\n\n", expression);
    
    // --- Tokenization Step ---

    // We need a temporary buffer to hold the tokens.
    // Let's assume a reasonable max number of tokens.
    int num_tokens = 0;
    char** tokens = malloc(sizeof(char*) * MAX_TOKENS);
    if (tokens == NULL) {
        fprintf(stderr, "Failed to allocate memory for tokens.\n");
        return 1;
    }

    // Call your new tokenizer
    tokenize(expression, tokens, &num_tokens);


    // --- Verification Step ---

    printf("Generated Tokens (%d total):\n", num_tokens);
    for (int i = 0; i < num_tokens; i++) {
        printf("  Token %2d: \"%s\"\n", i, tokens[i]);
    }


    // --- Cleanup Step ---

    printf("\n--- Cleaning up memory ---\n");
    // Free each individual token string that was allocated by the lexer
    for (int i = 0; i < num_tokens; i++) {
        free(tokens[i]);
        tokens[i] = NULL;
    }
    // Finally, free the array that held the pointers to the tokens
    free(tokens);
    tokens = NULL;
    
    printf("Test complete. All memory freed.\n");

    return 0;
    #endif
}