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


    char **grid = malloc(sizeof(char*)*grid_size_j);
    for (size_t i=0; i<grid_size_i; i++) {
        char *row = malloc(sizeof(char)*grid_size_i);
        grid[i] = row;
    }

    for (size_t i=0; i<grid_size_i; i++) {
        for (size_t j=0; j<grid_size_j; j++) {
            grid[i][j] = '_';
        }
    }

    for (size_t j=0; j<grid_size_j; j++) {
        for (size_t i=0; i<grid_size_i; i++) {
            printf("%c ", grid[i][j]);
        }
        printf("\n");
    }

    return EXIT_SUCCESS;
}
