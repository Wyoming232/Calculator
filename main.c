#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include <ctype.h>
#include "calculator.h"

#define INFIXPOSTFIX

int main() {
    // char *buffer = malloc(MAX_TOKENS * sizeof(char));
    // int result;

    #ifdef CALCULATOR
    printf("Welcome to calculator. Please enter your expression on the next line. Ensure that your expression uses spaces to distinguish between numbers and operators. Type ""exit"" to leave the program\n");
    //calculator loop until input is the string defined by EXIT condition
    while(strcmp(buffer, EXIT) == 0){
        scanf(">> %s", buffer);
        result = calculator(buffer);
        printf("Result : %.5f\n", &result);
    }
    #endif

    
    // TO test the infix to postfix conversion
    #ifdef INFIXPOSTFIX
    // Test 5: Right-to-Right Associativity (CRITICAL TEST)
    char *test5 = "10 * ( 2 + ( ( 6 - 4 ) * 3 ) )";
    printf("--- Test: Complex Expression ---\n");
    printf("Infix   : \"%s\"\n", test5);
    printf("Expected: 10 2 6 4 - 3 * + *\n");
    printf("Actual  : ");
    calculator(test5);
    printf("\n\n");
    #endif
}