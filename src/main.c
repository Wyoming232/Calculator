#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include <ctype.h>
#include "calculator.h"

#define CALCULATOR

typedef struct {
    char* title;
    char* expression;
} LexerTestCase;

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
    LexerTestCase test_suite[] = {
        {"Simple Expression", "10+5"},
        {"Complex Expression", "5*(100+var)/2^3"},
        {"Expression with Decimals", "3.14*2.5"},
        {"Multiple Operators", "1+2-3*4/5^6"},
        {"Nested Parentheses", "((25*30)+(34*2))"},
        {"Adjacent Parentheses", "(5+3)(10-2)"},
        {"Invalid Characters", "10 # 20 $ 5"}
    };
    int num_tests = sizeof(test_suite) / sizeof(test_suite[0]);

    printf("--- Running Lexer Test Suite ---\n\n");

    // Loop through all the defined test cases
    for (int i = 0; i < num_tests; i++) {
        LexerTestCase current_test = test_suite[i];
        printf("======================================\n");
        printf("Test %d: %s\n", i + 1, current_test.title);
        printf("Input   : \"%s\"\n", current_test.expression);
        printf("--------------------------------------\n");

        // --- Tokenization Step for each test ---
        int num_tokens = 0;
        char** tokens = malloc(sizeof(char*) * MAX_TOKENS);
        if (tokens == NULL) {
            fprintf(stderr, "Failed to allocate memory for tokens.\n");
            continue; // Skip to the next test
        }

        // Call your new tokenizer
        tokenize(current_test.expression, tokens, &num_tokens);

        // --- Verification Step ---
        printf("Output Tokens (%d total):\n", num_tokens);
        for (int j = 0; j < num_tokens; j++) {
            printf("  Token %2d: \"%s\"\n", j, tokens[j]);
        }

        // --- Cleanup Step for each test ---
        // Free each individual token string
        for (int j = 0; j < num_tokens; j++) {
            free(tokens[j]);
            tokens[j] = NULL;
        }
        // Finally, free the array that held the pointers
        free(tokens);
        tokens = NULL;
        
        printf("======================================\n\n");
    }
    
    printf("All lexer tests complete.\n");

    return 0;
    #endif
}