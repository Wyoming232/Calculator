#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include "calculator.h"

int main() {
    char buffer[] = malloc(sizeof(char) * MAX_TOKENS);
    int result;

    printf("Welcome to calculator. Please enter your expression on the next line. Ensure that your expression uses spaces to distinguish between numbers and operators. Type ""exit"" to leave the program\n");

    //calculator loop until input is the string defined by EXIT condition
    while(strcmp(buffer, EXIT) == 0){
        scanf(">> %s", buffer);
        result = calculator(buffer);
        printf("Result : %.5f\n", &result);
    }
}