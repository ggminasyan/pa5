#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "pish.h"

void parse_input(char *input, struct pish_arg *args) {
    args->argc = 0;
    char *token = strtok(input, " \t\n");
    while (token && args->argc < MAX_ARGC - 1) {
        args->argv[args->argc++] = token;
        token = strtok(NULL, " \t\n");
    }
    args->argv[args->argc] = NULL; // Null-terminate argv
}

void execute_command(struct pish_arg *args) {
    if (args->argc == 0) return; // Empty command
    if (strcmp(args->argv[0], "exit") == 0 || strcmp(args->argv[0], "cd") == 0 || strcmp(args->argv[0], "history") == 0) {
        handle_builtin(args);
        return;
    }

    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        execvp(args->argv[0], args->argv);
        perror("pish");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Parent process
        int status;
        wait(&status);
    } else {
        perror("fork");
    }
}

void handle_builtin(struct pish_arg *args) {
    if (strcmp(args->argv[0], "exit") == 0) {
        if (args->argc > 1) {
            fprintf(stderr, "Usage: exit\n");
        } else {
            exit(EXIT_SUCCESS);
        }
    } else if (strcmp(args->argv[0], "cd") == 0) {
        if (args->argc != 2) {
            fprintf(stderr, "Usage: cd <directory>\n");
        } else if (chdir(args->argv[1]) != 0) {
            perror("cd");
        }
    } else if (strcmp(args->argv[0], "history") == 0) {
        if (args->argc > 1) {
            fprintf(stderr, "Usage: history\n");
        } else {
            display_history();
        }
    }
}

int main(int argc, char *argv[]) {
    char input[MAX_CMD_LENGTH];
    struct pish_arg args;
    FILE *input_stream = stdin;

    if (argc == 2) {
        input_stream = fopen(argv[1], "r");
        if (!input_stream) {
            perror("open");
            return EXIT_FAILURE;
        }
    } else if (argc > 2) {
        fprintf(stderr, "Usage: %s [script_file]\n", argv[0]);
        return EXIT_FAILURE;
    }

    while (1) {
        if (input_stream == stdin) printf("▶ ");
        if (!fgets(input, sizeof(input), input_stream)) break;

        // Add non-empty commands to history
        if (strspn(input, " \t\n") != strlen(input)) {
            add_to_history(input);
        }

        parse_input(input, &args);
        execute_command(&args);
    }

    if (input_stream != stdin) fclose(input_stream);
    return EXIT_SUCCESS;
}

