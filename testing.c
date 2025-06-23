// ========================================================================
// === REPLACE YOUR OLD `while` LOOP WITH THIS NEW, CORRECTED VERSION ===
// ========================================================================

// (Make sure you have your helper functions ready)
// int get_precedence(char* op_str);
// int is_left_associative(char* op_str);

while (curr_pos_tokens < num_tokens) {
    char* token = tokens[curr_pos_tokens];
    int token_precedence = get_precedence(token);

    // Case 1: The token is a number (or a variable)
    if (token_precedence == 0) { // Assuming get_precedence returns 0 for non-operators
        arr[arr_pos++] = strdup(token);
    }
    // Case 2: The token is an open parenthesis '('
    else if (strcmp(token, "(") == 0) {
        push(stack, token);
    }
    // Case 3: The token is a close parenthesis ')'
    else if (strcmp(token, ")") == 0) {
        // Pop from the operator stack onto the output queue until we see an open parenthesis
        while (!isEmpty(stack) && strcmp(peek(stack), "(") != 0) {
            char* popped;
            pop(stack, &popped);
            arr[arr_pos++] = popped;
        }
        // Now, pop the open parenthesis '(' from the stack and discard it.
        if (!isEmpty(stack)) {
            char* popped;
            pop(stack, &popped); 
            // We don't add the parenthesis to the output array.
            // If you allocated memory for it, you might need to free it here.
        }
    }
    // Case 4: The token is an operator (+, -, *, /, ^)
    else {
        // This is the key logic with the WHILE loop
        while (!isEmpty(stack) && get_precedence(peek(stack)) > 0) { // While top is an operator
            char* o2 = peek(stack);
            
            // Here is the combined precedence and associativity check
            if ( (is_left_associative(token) && get_precedence(token) <= get_precedence(o2)) ||
                 (!is_left_associative(token) && get_precedence(token) < get_precedence(o2)) ) {
                
                char* popped;
                pop(stack, &popped);
                arr[arr_pos++] = popped;
            } else {
                // The condition failed, so we stop popping.
                break;
            }
        }
        // After the loop, push the current operator token onto the stack.
        push(stack, token);
    }

    curr_pos_tokens++;
}

// (The rest of your code to empty the stack remains the same)
// ...