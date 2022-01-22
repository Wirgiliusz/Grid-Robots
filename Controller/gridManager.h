#include <stdio.h>

#define INPUT_BUFFER_SIZE 1024

struct GridManager {
    FILE *file;

    char msg_grid_size[INPUT_BUFFER_SIZE];
    char msg_storages_points[INPUT_BUFFER_SIZE];
    char msg_items_points[INPUT_BUFFER_SIZE];
    char msg_robots_points[INPUT_BUFFER_SIZE];

    size_t grid_size_i;
    size_t grid_size_j;
    char **grid;
};

int readInputData(struct GridManager *gm, const char *file_name);

void printGrid(struct GridManager *gm);
char* planPath(struct GridManager *gm);
