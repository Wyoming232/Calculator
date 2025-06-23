#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include <ctype.h>
#include "calculator.h"

#define CALCULATOR

typedef struct {
        const char* title;
        char* infix;
        char* expected_postfix;
    } ComprehensiveTestCase;

void run_comprehensive_test_suite() {
    printf("====================================================\n");
    printf("      RUNNING COMPREHENSIVE TEST SUITE\n");
    printf("  (Please manually verify the 'Actual' output)\n");
    printf("====================================================\n\n");

    // A comprehensive array of test cases
    ComprehensiveTestCase test_cases[] = {
        // --- PRECEDENCE ---
        {"Test: Precedence (PEMDAS)", "10 + 20 * 3", "10 20 3 * +"},
        {"Test: Precedence (Division)", "100 / 4 - 5", "100 4 / 5 -"},
        {"Test: Precedence (Exponent)", "5 * 2 ^ 4", "5 2 4 ^ *"},
        {"Test: Precedence (Mixed)", "20 - 3 * 2 ^ 3 + 1", "20 3 2 3 ^ * - 1 +"},

        // --- ASSOCIATIVITY ---
        {"Test: Left-Associativity", "100 - 50 + 10 - 5", "100 50 - 10 + 5 -"},
        {"Test: Right-Associativity (Canonical)", "3 ^ 2 ^ 3", "3 2 3 ^ ^"},
        {"Test: Right-Associativity (Complex)", "2 ^ 3 ^ 2 ^ 1", "2 3 2 1 ^ ^ ^"},

        // --- PARENTHESES ---
        {"Test: Simple Parentheses", "( 10 + 20 ) * 3", "10 20 + 3 *"},
        {"Test: Nested Parentheses", "10 * ( 5 + ( 30 / 3 ) )", "10 5 30 3 / + *"},
        {"Test: Complex Parentheses", "5 * ( ( 100 - 90 ) ^ 2 / 5 )", "5 100 90 - 2 ^ 5 / *"},
        {"Test: Multiple Parentheses Groups", "( 8 + 2 ) * ( 9 - 4 )", "8 2 + 9 4 - *"},
        {"Test: Deeply Nested", "( ( ( 1 + 1 ) * 2 ) ^ 2 ) / 8", "1 1 + 2 * 2 ^ 8 /"},
        
        // --- EDGE CASES ---
        {"Test: Edge Case (Single Number)", "42", "42"}
    };

    int num_tests = sizeof(test_cases) / sizeof(test_cases[0]);

    for (int i = 0; i < num_tests; i++) {
        ComprehensiveTestCase test = test_cases[i];
        
        printf("--- %s ---\n", test.title);
        printf("Infix   : \"%s\"\n", test.infix);
        printf("Expected: %s\n", test.expected_postfix);
        printf("Actual  : ");
        
        // Your function is called here to print the result
        calculator(test.infix);
        
        printf("\n\n");
    }

    printf("====================================================\n");
    printf("              ALL TESTS COMPLETE\n");
    printf("====================================================\n");
}

int main() {
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

    
    // TO test the infix to postfix conversion
    #ifdef INFIXPOSTFIX
    run_comprehensive_test_suite();
    #endif
}