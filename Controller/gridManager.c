#include "gridManager.h"
#include "pathPlanner.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>


static int openFile(struct GridManager *gm, const char *file_name);
static void readGridDimensions(struct GridManager *gm);
static void createEmptyGrid(struct GridManager *gm);
static void readAndAddStorages(struct GridManager *gm);
static void readAndAddItems(struct GridManager *gm);
static void readAndAddRobots(struct GridManager *gm);

static void constructMessages(struct GridManager *gm);
static void addGridSizeMessege(struct GridManager *gm);
static void addRobotsPointsMessege(struct GridManager *gm, size_t robot_position_i, size_t robot_position_j, size_t msg_idx);
static void addItemsPointsMessege(struct GridManager *gm, size_t item_position_i, size_t item_position_j, size_t msg_idx);
static void addStoragesPointsMessege(struct GridManager *gm, size_t storage_position_i, size_t storage_position_j, size_t msg_idx);


int readInputData(struct GridManager *gm, const char *file_name) {
    if(!openFile(gm, file_name)) {
        perror("File opening failed");
        return -1;
    }

    readGridDimensions(gm);

    readAndAddStorages(gm);
    readAndAddItems(gm);
    readAndAddRobots(gm);

    printGrid(gm);
    constructMessages(gm);

    return 0;
}


static int openFile(struct GridManager *gm, const char *file_name) {
    gm->file = fopen(file_name, "r");
    if (!gm->file) {
        perror("File opening failed");
        return 0;
    } else {
        return 1;
    }
}

static void readGridDimensions(struct GridManager *gm) {
    char input_line[INPUT_BUFFER_SIZE];

    fgets(input_line, INPUT_BUFFER_SIZE, gm->file);
    char *token = strtok(input_line, " ");
    gm->grid_size_i = atoi(token);
    token = strtok(NULL, " ");
    gm->grid_size_j = atoi(token);

    createEmptyGrid(gm);
}

static void createEmptyGrid(struct GridManager *gm) {
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

static void readAndAddStorages(struct GridManager *gm) {
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

static void readAndAddItems(struct GridManager *gm) {
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

static void readAndAddRobots(struct GridManager *gm) {
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

static void constructMessages(struct GridManager *gm) {
    size_t msg_robots_idx = 0;
    size_t msg_items_idx = 0;
    size_t msg_storages_idx = 0;

    addGridSizeMessege(gm);

    for (size_t j=0; j<gm->grid_size_j; j++) {
        for (size_t i=0; i<gm->grid_size_i; i++) {
            if (isdigit(gm->grid[i][j])) {
                addRobotsPointsMessege(gm, i, j, msg_robots_idx);
                msg_robots_idx += 4;
            } else if (gm->grid[i][j] == 'I') {
                addItemsPointsMessege(gm, i, j, msg_items_idx);
                msg_items_idx += 4;
            } else if (gm->grid[i][j] == 'S') {
                addStoragesPointsMessege(gm, i, j, msg_storages_idx);
                msg_storages_idx += 4;
            }
        }
    }

    gm->msg_robots_points[msg_robots_idx] = '\n';
    gm->msg_robots_points[msg_robots_idx + 1] = '\0';

    gm->msg_items_points[msg_items_idx] = '\n';
    gm->msg_items_points[msg_items_idx + 1] = '\0';

    gm->msg_storages_points[msg_storages_idx] = '\n';
    gm->msg_storages_points[msg_storages_idx + 1] = '\0';
}

static void addGridSizeMessege(struct GridManager *gm) {
    gm->msg_grid_size[0] = gm->grid_size_i + '0';
    gm->msg_grid_size[1] = ' ';
    gm->msg_grid_size[2] = gm->grid_size_j + '0';
    gm->msg_grid_size[3] = '\n';
    gm->msg_grid_size[4] = '\0';
}

static void addRobotsPointsMessege(struct GridManager *gm, size_t robot_position_i, size_t robot_position_j, size_t msg_idx) {
    gm->msg_robots_points[msg_idx] = robot_position_i + '0';
    gm->msg_robots_points[msg_idx + 1] = ' ';
    gm->msg_robots_points[msg_idx + 2] = robot_position_j + '0';
    gm->msg_robots_points[msg_idx + 3] = ' ';
}

static void addItemsPointsMessege(struct GridManager *gm, size_t item_position_i, size_t item_position_j, size_t msg_idx) {
    gm->msg_items_points[msg_idx] = item_position_i + '0';
    gm->msg_items_points[msg_idx + 1] = ' ';
    gm->msg_items_points[msg_idx + 2] = item_position_j + '0';
    gm->msg_items_points[msg_idx + 3] = ' ';
}

static void addStoragesPointsMessege(struct GridManager *gm, size_t storage_position_i, size_t storage_position_j, size_t msg_idx) {
    gm->msg_storages_points[msg_idx] = storage_position_i + '0';
    gm->msg_storages_points[msg_idx + 1] = ' ';
    gm->msg_storages_points[msg_idx + 2] = storage_position_j + '0';
    gm->msg_storages_points[msg_idx + 3] = ' ';
}

void printGrid(struct GridManager *gm) {
    for (size_t j=0; j<gm->grid_size_j; j++) {
        for (size_t i=0; i<gm->grid_size_i; i++) {
            printf("%c ", gm->grid[i][j]);
        }
        printf("\n");
    }
}

char* planPath(struct GridManager *gm) {
    size_t robot_i = 99;
    size_t robot_j = 99;
    size_t item_i = 99;
    size_t item_j = 99;
    size_t storage_i = 99;
    size_t storage_j = 99;

    for (size_t j=0; j<gm->grid_size_j; j++) {
        for (size_t i=0; i<gm->grid_size_i; i++) {
            if (robot_i != 99 && item_i != 99 && storage_i != 99) {
                break;
            }
            
            if (isdigit(gm->grid[i][j])) {
                robot_i = i;
                robot_j = j;
            } else if (gm->grid[i][j] == 'I') {
                item_i = i;
                item_j = j;
            } else if (gm->grid[i][j] == 'S') {
                storage_i = i;
                storage_j = j;
            }
        }

        if (robot_i != 99 && item_i != 99 && storage_i != 99) {
            break;
        }
    } 

    if (robot_i != 99 && item_i != 99 && storage_i != 99) {
        gm->grid[robot_i][robot_j] = '_';
        gm->grid[item_i][item_j] = '_';
        gm->grid[storage_i][storage_j] = '_';

        return constructPathThroughPoint(robot_i, robot_j, storage_i, storage_j, item_i, item_j);
    } else {
        return NULL;
    }
}

void recoverRobot(struct GridManager *gm, size_t robot_i, size_t robot_j) {
    gm->grid[robot_i][robot_j] = '0';
}