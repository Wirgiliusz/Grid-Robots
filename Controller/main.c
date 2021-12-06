#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_BUFFER_SIZE 1024

int main() {
    printf("Hello Controller!\n");

    FILE *file;
    file = fopen("input.txt", "r");
    if (!file) {
        perror("File opening failed");
        return EXIT_FAILURE;
    }

    char input_line[INPUT_BUFFER_SIZE];

    size_t grid_size_i;
    size_t grid_size_j;
    fgets(input_line, INPUT_BUFFER_SIZE, file);
    char *token = strtok(input_line, " ");
    grid_size_i = atoi(token);
    token = strtok(NULL, " ");
    grid_size_j = atoi(token);


    return EXIT_SUCCESS;
}