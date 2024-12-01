#ifndef PISH_H
#define PISH_H

#define MAX_ARGC 64
#define MAX_CMD_LENGTH 1024

struct pish_arg {
    int argc;
    char *argv[MAX_ARGC];
};

// Function declarations
void parse_input(char *input, struct pish_arg *args);
void execute_command(struct pish_arg *args);
void handle_builtin(struct pish_arg *args);
void add_to_history(const char *command);
void display_history();
char *get_history_path();

#endif // PISH_H

