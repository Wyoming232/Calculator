#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include "calculator.h"

int main() {
    char s[] = malloc(sizeof(char) * MAX_TOKENS);
    float result;

    printf("Welcome to calculator. Please enter your expression on the next line. Ensure that your expression uses spaces to distinguish between numbers and operators. Type ""exit"" to leave the program\n");

    //calculator loop until input is the string defined by EXIT condition
    while(strcmp(s, EXIT) == 0){
        scanf(">> %s", s);
        result = calculator(s);
        printf("Result : %.5f\n", &result);
    }
}