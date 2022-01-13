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

int openFile(struct GridManager *gm, const char *file_name);
void readGridDimensions(struct GridManager *gm);
void createEmptyGrid(struct GridManager *gm);
void readAndAddStorages(struct GridManager *gm);
void readAndAddItems(struct GridManager *gm);
void readAndAddRobots(struct GridManager *gm);
void printGrid(struct GridManager *gm);

void constructMessages(struct GridManager *gm);
void addGridSizeMessege(struct GridManager *gm);
void addRobotsPointsMessege(struct GridManager *gm, size_t robot_position_i, size_t robot_position_j, size_t msg_idx);
void addItemsPointsMessege(struct GridManager *gm, size_t item_position_i, size_t item_position_j, size_t msg_idx);
void addStoragesPointsMessege(struct GridManager *gm, size_t storage_position_i, size_t storage_position_j, size_t msg_idx);

