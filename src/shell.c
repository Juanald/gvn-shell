#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <unistd.h>
#include <limits.h> 

#define BUFF_SIZE 50
#define MAX_ARGS 10
#define MAX_ARG_SIZE 100

void gvn_loop(void);
char* gvn_read_line(void);
char** gvn_split_line(char* line);
int calculate_string_array_size(char** array);
int gvn_execute_args(char** args);
int gvn_execute_ls(char** args);
char** slice_args(char** args, int beginning, int end);
int gvn_execute_cat(char** args);
int gvn_execute_cp(char** args);
int gvn_execute_echo(char** args);

int main(int argc, char* argv[]) {
    // Load config

    gvn_loop();

    // Perform shutdown
    return 0;
}

void gvn_loop(void) {
    char* line;
    char** args;
    int status;

    // We loop through once. We should read from stdin, parse the line, and execute arguments. We should do this as long as we haven't terminated
    do {
        line = gvn_read_line();
        if (strcmp(line, "q\n") == 0) {
            exit(EXIT_SUCCESS);
        }
        args = gvn_split_line(line);

        status = gvn_execute_args(args);
    } while (status);   
}

char* gvn_read_line(void) {
    char* line = malloc(sizeof(char) * BUFF_SIZE);
    fgets(line, 50, stdin);
    return line;
}

/*
A function that takes in a string, and splits it into an array of arguments. The first argument is always the command, and the remaining arguments are the arguments to that command. 
*/
char** gvn_split_line(char* line) {
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

// This is a function that executes the command provided in the args array. Note that args[0] holds the command. Let's implement ls
int gvn_execute_args(char** args) {
    char* command = args[0];

    // ls command
    if (strcmp(command, "ls") == 0) {
        return gvn_execute_ls(slice_args(args, 1, calculate_string_array_size(args)));
    }
    else if (strcmp(command, "cat") == 0) {
        return gvn_execute_cat(slice_args(args, 1, calculate_string_array_size(args)));
    }
    else if (strcmp(command, "cp") == 0) {
        return gvn_execute_cp(slice_args(args, 1, calculate_string_array_size(args)));
    }
    else if (strcmp(command, "pwd") == 0) {
        char cwd[MAX_PATH];
        printf("%s\n", getcwd(cwd, sizeof(cwd)));
        return 1;
    }
    else if (strcmp(command, "clear") == 0) {
        system("cls");
        return 1;
    }
    else if (strcmp(command, "echo") == 0) {
        return gvn_execute_echo(slice_args(args,1, calculate_string_array_size(args)));
    }
    else {
        printf("Unrecognized command\n");
        return 1;
    }
}

int calculate_string_array_size(char** array) {
    int size = 0;
    while (array[size] != NULL) {
        size++;
    }
    return size;
}

// A function that slices a string array
char** slice_args(char** array, int beginning, int end) {
    char** slice = malloc(sizeof(char*) * MAX_ARGS); // Allocate an array of strings
    int count = 0;
    for (int i = beginning; i < end; i++) {
        slice[count] = malloc(sizeof(char) * MAX_ARG_SIZE); // Allocate a buffer to store strings in
        strcpy(slice[count], array[i]); // Store th strings
        count++;
    }
    slice[count] = NULL; // Required for a dynamically sized array for slice function
    return slice;
}

// Recreates ls. Lists out all directories/files with the given filename. No wildcards yet, only for windows
int gvn_execute_ls(char** args) {
    WIN32_FIND_DATAA file_found; // Stores found file data
    HANDLE hFind; // Stores handle data returned by FindFile functions

    if (calculate_string_array_size(args) == 0) {
        hFind = FindFirstFile("*", &file_found);
        do {
            // Make sure the file is not an ancestor
            if (strcmp(".", file_found.cFileName) != 0 && strcmp("..", file_found.cFileName) != 0) {
                printf("%s\n", file_found.cFileName);
            }
        } while(FindNextFile(hFind, &file_found) != 0);
        FindClose(hFind);
        return 1;
    } else if (calculate_string_array_size(args) > 0) {
        for (int i = 0; i < calculate_string_array_size(args); i++) {
            hFind = FindFirstFile(args[i], &file_found);
            printf("%s\n", file_found.cFileName);
        }
        FindClose(hFind);
    } else {
        printf("Usage: ls arg1 arg2...");
    }
    return 1;
}

// Implements the cat function for multiple arguments
int gvn_execute_cat(char** args) {
    FILE* f;
    char string_buffer[100];
    for (int i = 0; i < calculate_string_array_size(args); i++) {
        f = fopen(args[i], "r");
        if (f == NULL) {
            printf("Failed to open %s\n", args[i]);
        }
        while(fgets(string_buffer, sizeof(string_buffer), f) != NULL) {
            printf("%s", string_buffer);
        }
        printf("\n");
        fclose(f);
    } 
    return 1;
}

// This function copies a file args[0] and puts it into another file args[1]
int gvn_execute_cp(char** args) {
    FILE* fin = fopen(args[0], "r");
    FILE* fout = fopen(args[1], "w");
    if (fin == NULL) {
        printf("Couldn't find file %s", args[0]);
        return 1;
    }
    if (fout == NULL) {
        printf("Couldn't open new file %s", args[1]);
        return 1;
    }

    // Initialize a buffer to hold the file string
    char* buffer = malloc(sizeof(char) * 1024);
    if (buffer == NULL) {
        printf("Buffer memory allocation failed");
        fclose(fin);
        fclose(fout);
        return 1;
    }
    int char_count = 0;
    char c;

    // Read a char until EOF. If buffer runs out of memory, reallocate. Store char into buffer
    while ((c = fgetc(fin)) != EOF) {
        buffer[char_count] = c;
        char_count++;
        if (char_count > 1024) {
            buffer = realloc(buffer, sizeof(char) * char_count + 1024);
            if (buffer == NULL) {
                printf("Buffer memory allocation failed");
                fclose(fin);
                fclose(fout);
                return 1;
            }
        }
    }
    // Write to the output file
    fwrite(buffer, sizeof(char), char_count, fout);

    free(buffer);
    fclose(fin);
    fclose(fout);
    return 1;
}

int gvn_execute_echo(char** args) {
    for (int arg = 0; arg < calculate_string_array_size(args); arg++) {
        if (args[arg][0] == '"') {
            int char_count = 1;
            while (args[arg][char_count] != '"') {
                printf("%c", args[arg][char_count]);
                char_count++;
            }
            char_count = 1;
            printf("\n");
        } else {
            printf("%s\n", args[arg]);
        }
    }
    return 1;
}