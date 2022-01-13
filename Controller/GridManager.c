#include "GridManager.h"
#include <string.h>
#include <stdlib.h>

int openFile(struct GridManager *gm, const char *file_name) {
    gm->file = fopen(file_name, "r");
    if (!gm->file) {
        perror("File opening failed");
        return 0;
    } else {
        return 1;
    }
}

void readGridDimensions(struct GridManager *gm) {
    char input_line[INPUT_BUFFER_SIZE];

    fgets(input_line, INPUT_BUFFER_SIZE, gm->file);
    char *token = strtok(input_line, " ");
    gm->grid_size_i = atoi(token);
    token = strtok(NULL, " ");
    gm->grid_size_j = atoi(token);

    createEmptyGrid(gm);
}

void createEmptyGrid(struct GridManager *gm) {
    char **grid = malloc(sizeof(char*)*gm->grid_size_j);
    for (size_t i=0; i<gm->grid_size_i; i++) {
        char *row = malloc(sizeof(char)*gm->grid_size_i);
        grid[i] = row;
    }

    for (size_t i=0; i<gm->grid_size_i; i++) {
        for (size_t j=0; j<gm->grid_size_j; j++) {
            grid[i][j] = '_';
        }
    }

    gm->grid = grid;
}

void readAndAddStorages(struct GridManager *gm) {
    char input_line[INPUT_BUFFER_SIZE];
    size_t x;
    size_t y;

    fgets(input_line, INPUT_BUFFER_SIZE, gm->file);
    char *token = strtok(input_line, " ");
    while (token) {
        token = strtok(NULL, " ");
        if (!token) {
            break;
        }
        x = atoi(token);
        token = strtok(NULL, " ");
        y = atoi(token);

        gm->grid[x][y] = 'S';
    }
}

void readAndAddItems(struct GridManager *gm) {
    char input_line[INPUT_BUFFER_SIZE];
    size_t x;
    size_t y;

    fgets(input_line, INPUT_BUFFER_SIZE, gm->file);
    char *token = strtok(input_line, " ");
    while (token) {
        token = strtok(NULL, " ");
        if (!token) {
            break;
        }

        x = atoi(token);
        token = strtok(NULL, " ");
        y = atoi(token);

        gm->grid[x][y] = 'I';
    }
}

void readAndAddRobots(struct GridManager *gm) {
    char input_line[INPUT_BUFFER_SIZE];
    size_t x;
    size_t y;
    int robot_number = 0;

    fgets(input_line, INPUT_BUFFER_SIZE, gm->file);
    char *token = strtok(input_line, " ");
    while (token) {
        token = strtok(NULL, " ");
        if (!token) {
            break;
        }

        x = atoi(token);
        token = strtok(NULL, " ");
        y = atoi(token);

        gm->grid[x][y] = robot_number + '0';
        robot_number++;
    }
}
