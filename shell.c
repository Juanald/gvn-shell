#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 50
#define MAX_ARGS 10
#define MAX_ARG_SIZE 100

void lsh_loop(void);
char* lsh_read_line(void);
char** lsh_split_line(char* line);
int calculate_string_array_size(char** array);

int main(int argc, char* argv[]) {
    // Load config

    lsh_loop();

    // Perform shutdown
    return 0;
}

void lsh_loop(void) {
    char* line;
    char** args;
    int status;

    // We loop through once. We should read from stdin, parse the line, and execute arguments. We should do this as long as we haven't terminated
    do {
        line = lsh_read_line();
        if (strcmp(line, "q\n") == 0) {
            exit(EXIT_SUCCESS);
        }
        // printf("%s", line);
        args = lsh_split_line(line);
        
        // Print each argument (for debugging)
        // for (int i = 0; args[i] != NULL; i++) {
        //     printf("arg[%d]: %s\n", i, args[i]);
        // }
        // status = lsh_execute_args(args);
    } while (status);   
}

char* lsh_read_line(void) {
    char* line = malloc(sizeof(char) * BUFF_SIZE);
    fgets(line, 50, stdin);
    return line;
}

/*
A function that takes in a string, and splits it into an array of arguments. The first argument is always the command, and the remaining arguments are the arguments to that command. 
*/
char** lsh_split_line(char* line) {
    // We have to initialize an array of strings to hold memory
    char** args = malloc(sizeof(char*) * MAX_ARGS);
    char* tempString = malloc(sizeof(char) * MAX_ARG_SIZE);
    
    int argc = 0;
    int tempIndex = 0;

    // We parse the line, breaking at spaces and storing it into the args buffer
    for (int i = 0; i < strlen(line); i++) {
        if (line[i] == ' ' || line[i] == '\n' || line[i] == EOF) {
            tempString[tempIndex] = '\0'; // Null terminate the string
            // We create a buffer at the memory location, and copy in the tempString holding the user's argcth arg
            args[argc] = malloc(sizeof(char) * MAX_ARG_SIZE);
            // Copy into the args array the temporary string
            strcpy(args[argc], tempString);
            argc++;
            // Clear temp string
            memset(tempString, 0, MAX_ARG_SIZE);
            tempIndex = 0;
        } else {
            // Add to the tempString buffer
            tempString[tempIndex] = line[i];
            tempIndex++;
        }
    }
    free(tempString);
    args[argc] = NULL;
    return args;
}

int calculate_string_array_size(char** array) {
    int size = 0;
    while (array[size] != NULL) {
        size++;
    }
    return size;
}