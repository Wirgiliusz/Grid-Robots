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

    int fd_input;
    int fd_output;
    const char *fifo_output_path;
    const char *fifo_input_path;

    size_t free_robots;
};

int readInputData(struct GridManager *gm, const char *file_name);
void createFifos(struct GridManager *gm, const char* output_fifo_path, const char* input_fifo_path);
int writeInputData(struct GridManager *gm);

void printGrid(struct GridManager *gm);
int scanAndPlan(struct GridManager *gm);
int readAndRecover(struct GridManager *gm);
