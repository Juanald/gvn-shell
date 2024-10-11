// C program to illustrate the implementation of lexical
// analyser. Identifies delimeters, identifiers (variable names), operators, keywords, integers, 

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100

bool is_operator(char c);

// This is a function that identifies a delimeter
bool is_delimeter(char c) {
    char delimiters[] = {';', ' ', ',', '(', ')', '{', '}', '[', ']', '"', '\'', '#', ':', '.', '<', '>', '\\', '\0'};
    return (strchr(delimiters, c));
}

// This is a function that identifies an identifier (doesn't start with a digit, not a delimeter)
bool is_identifier(char* c) {
    for (int i = 0; i < strlen(c); i++) {
        if (is_delimeter(c[i]) || is_operator
        (c[i])) return false;
    }
    return (!isdigit(c[0]));
}

// Checks if a given string is meant to be a number or an integer
bool is_integer(char* str) {
    int i;
    if (str == NULL || str == '\0') return false;
    for (i = 0; i < strlen(str); i++) {
        if (!isdigit(str[i])) return false;
    }
    return (str[i] == '\0'); // If terminates with a null character, it is a string
}

// Checks if a string is a pre-defined keyword
bool is_keyword(char* str) {
    char* keywords[]
            = { "auto",     "break",    "case",     "char",
            "const",    "continue", "default",  "do",
            "double",   "else",     "enum",     "extern",
            "float",    "for",      "goto",     "if",
            "int",      "long",     "register", "return",
            "short",    "signed",   "sizeof",   "static",
            "struct",   "switch",   "typedef",  "union",
            "unsigned", "void",     "volatile", "while" };
    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return true;
        }
    }
    return false;
}

// Checks if an operator, basic math
bool is_operator(char c) {
    if (c == '\0') return false;
    char operators[] = {'+', '-', '*', '/', '=', '\0'};
    return strchr(operators, c);
}

char* get_substring(char* str, int l, int r) {
    int sub_length = r - l;
    // We allocate a memory buffer
    char* slice = calloc(sizeof(char), sub_length + 1); // we have space for the null string
    strncpy(slice, str + l, sub_length); // Black magic
    slice[sub_length] = '\0';
    return slice;
}

int lexical_analyser(char* str) {
    // Need two pointers to be able to slice adequately.
    int left = 0, right = 0;
    char* subString;
    while (right < strlen(str) && left <= right) {
        // We break the string into lexemes
        
        if (is_delimeter(str[left])) {
            right++;
            left++;
        }

        // Surround a potential lexeme
        if (!is_delimeter(str[right])) right++;

        // Check if the substring is a reserved keyword
        if ((is_delimeter(str[right]) && left < right) || right == strlen(str) && left < right) {
            subString = get_substring(str, left, right);
            if (is_keyword(subString)) {
                printf("Reserved keyword detected: %s\n", subString);
                right++;
                left = right;
            }
        }

        // Slice the substring if an identifier
        if ((is_delimeter(str[right]) && left < right) || right == strlen(str) && left <= right) {
            subString = get_substring(str, left, right);
            // printf("%s\n", subString);
            if (is_identifier(subString)) {\
                printf("Identifer detected: %s\n", subString);
                right++;
                left = right;
            }
        }

        // Potential integer
        if ((is_delimeter(str[right]) && left < right) || right == strlen(str) && left < right) {
            subString = get_substring(str, left, right);
            if (is_integer(subString)) {
                printf("Integer detected: %s\n", subString);
                right++;
                left = right;
            }
        }

        // Potential operator
        if ((is_delimeter(str[right]) && is_operator(str[left])) || right == strlen(str) && left < right) {
            // printf("Left is: %d, right is %d\n", left, right);
            printf("Operator detected: %s\n", get_substring(str, left, right));
            right++;
            left = right;
        }
    }
    return 0;
} 

char* join_array(char** array, int array_size) {
    // First, calculate the total length needed for the final string
    int total_length = 0;
    for (int i = 1; i < array_size; i++) {
        total_length += strlen(array[i]);
    }

    // Allocate memory for the result string, including space for the null terminator
    char* str = malloc(total_length + 1 + sizeof(array)); // +1 for the null terminator
    if (str == NULL) {
        return NULL; // Handle memory allocation failure
    }

    // Initialize the string to be empty
    str[0] = '\0';

    // Concatenate each string from the array into the result string
    for (int i = 1; i < array_size; i++) {
        strcat(str, array[i]);
        strcat(str, " ");
    }

    return str;
}

int calculate_string_array_size(char** array) {
    int size = 0;
    while (array[size] != NULL) {
        size++;
    }
    return size;
}

int main(int argc, char** argv) {
    char delimeter = '{';
    char* identifier = "x123";
    char* integer = "123";
    char operator = '*';

    printf("%c resolved to %d\n", delimeter, is_delimeter(delimeter));
    printf("%s resolved to %d\n", identifier, is_identifier(identifier));
    printf("%s resolved to %d\n", integer, is_integer(integer));
    printf("%c resolved to %d\n", operator, is_operator(operator));
    printf("balls{ slices to %s\n", get_substring("balls{", 0, 5));
    lexical_analyser("{{{{{ballsyyyyyy}}}}} * 69 const");
    lexical_analyser(" / ");
    lexical_analyser("1234");
    lexical_analyser("a + b = c");
    lexical_analyser("d+e=f");
    printf("String is %s\n", join_array(argv, calculate_string_array_size(argv)));
    lexical_analyser(join_array(argv, calculate_string_array_size(argv)));
    return 0;
}