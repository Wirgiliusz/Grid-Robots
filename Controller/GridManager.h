#include <stdio.h>

#define INPUT_BUFFER_SIZE 1024

struct GridManager {
    FILE *file;

    char msg_grid_size[INPUT_BUFFER_SIZE];
    char msg_storages_points[INPUT_BUFFER_SIZE];
    char msg_items_points[INPUT_BUFFER_SIZE];
    char msg_robots_points[INPUT_BUFFER_SIZE];
};

int openFile(struct GridManager *gm, const char *file_name);

/*
void readGridDimensions(FILE *file, size_t *grid_size_i, size_t *grid_size_j);
char** createEmptyGrid(size_t grid_size_i, size_t grid_size_j);
void readAndAddStorages(FILE *file, char **grid);
void readAndAddItems(FILE *file, char **grid);
void readAndAddRobots(FILE *file, char **grid);
void printGrid(char **grid, size_t grid_size_i, size_t grid_size_j);
*/