#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_BUFFER_SIZE 1024

void readGridDimensions(FILE *file, size_t *grid_size_i, size_t *grid_size_j);
char** createEmptyGrid(size_t grid_size_i, size_t grid_size_j);
void readAndAddStorages(FILE *file, char **grid);
void readAndAddItems(FILE *file, char **grid);
void readAndAddRobots(FILE *file, char **grid);
void printGrid(char **grid, size_t grid_size_i, size_t grid_size_j);


int main() {
    printf("Hello Controller!\n");

    FILE *file;
    file = fopen("input.txt", "r");
    if (!file) {
        perror("File opening failed");
        return EXIT_FAILURE;
    }

    size_t grid_size_i = 0;
    size_t grid_size_j = 0;
    readGridDimensions(file, &grid_size_i, &grid_size_j);
    char **grid = createEmptyGrid(grid_size_i, grid_size_j);

    readAndAddStorages(file, grid);
    readAndAddItems(file, grid);
    readAndAddRobots(file, grid);

    printGrid(grid, grid_size_i, grid_size_j);


    return EXIT_SUCCESS;
}

void readGridDimensions(FILE *file, size_t *grid_size_i, size_t *grid_size_j) {
    char input_line[INPUT_BUFFER_SIZE];

    fgets(input_line, INPUT_BUFFER_SIZE, file);
    char *token = strtok(input_line, " ");
    *grid_size_i = atoi(token);
    token = strtok(NULL, " ");
    *grid_size_j = atoi(token);
}

char** createEmptyGrid(size_t grid_size_i, size_t grid_size_j) {
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

    return grid;
}

void readAndAddStorages(FILE *file, char **grid) {
    char input_line[INPUT_BUFFER_SIZE];
    size_t x;
    size_t y;

    fgets(input_line, INPUT_BUFFER_SIZE, file);
    char *token = strtok(input_line, " ");
    while (token) {
        token = strtok(NULL, " ");
        if (!token) {
            break;
        }
        x = atoi(token);
        token = strtok(NULL, " ");
        y = atoi(token);

        grid[x][y] = 'S';
    }
}

void readAndAddItems(FILE *file, char **grid) {
    char input_line[INPUT_BUFFER_SIZE];
    size_t x;
    size_t y;

    fgets(input_line, INPUT_BUFFER_SIZE, file);
    char *token = strtok(input_line, " ");
    while (token) {
        token = strtok(NULL, " ");
        if (!token) {
            break;
        }

        x = atoi(token);
        token = strtok(NULL, " ");
        y = atoi(token);

        grid[x][y] = 'I';
    }
}

void readAndAddRobots(FILE *file, char **grid) {
    char input_line[INPUT_BUFFER_SIZE];
    size_t x;
    size_t y;
    int robot_number = 0;

    fgets(input_line, INPUT_BUFFER_SIZE, file);
    char *token = strtok(input_line, " ");
    while (token) {
        token = strtok(NULL, " ");
        if (!token) {
            break;
        }

        x = atoi(token);
        token = strtok(NULL, " ");
        y = atoi(token);

        grid[x][y] = robot_number + '0';
        robot_number++;
    }
}

void printGrid(char **grid, size_t grid_size_i, size_t grid_size_j) {
    for (size_t j=0; j<grid_size_j; j++) {
        for (size_t i=0; i<grid_size_i; i++) {
            printf("%c ", grid[i][j]);
        }
        printf("\n");
    }
}