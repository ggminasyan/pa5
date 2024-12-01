#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pish.h"

char *get_history_path() {
    static char path[1024];
    snprintf(path, sizeof(path), "%s/.pish_history", getenv("HOME"));
    return path;
}

void add_to_history(const char *command) {
    if (strlen(command) == 0) return;
    FILE *file = fopen(get_history_path(), "a");
    if (file) {
        fprintf(file, "%s", command);
        fclose(file);
    }
}

void display_history() {
    FILE *file = fopen(get_history_path(), "r");
    if (!file) {
        perror("history");
        return;
    }
    char line[1024];
    int counter = 1;
    while (fgets(line, sizeof(line), file)) {
        printf("%d %s", counter++, line);
    }
    fclose(file);
}

