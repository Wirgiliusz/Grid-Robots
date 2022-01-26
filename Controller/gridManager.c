#include "gridManager.h"
#include "pathPlanner.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>


static int openFile(struct GridManager *gm, const char *file_name);
static void readGridDimensions(struct GridManager *gm);
static void createEmptyGrid(struct GridManager *gm);
static void readAndAddStorages(struct GridManager *gm);
static void readAndAddItems(struct GridManager *gm);
static void readAndAddRobots(struct GridManager *gm);
static int readConfirmation(struct GridManager *gm);

static void constructMessages(struct GridManager *gm);
static void addGridSizeMessege(struct GridManager *gm);
static void addRobotsPointsMessege(struct GridManager *gm, size_t robot_position_i, size_t robot_position_j, size_t msg_idx);
static void addItemsPointsMessege(struct GridManager *gm, size_t item_position_i, size_t item_position_j, size_t msg_idx);
static void addStoragesPointsMessege(struct GridManager *gm, size_t storage_position_i, size_t storage_position_j, size_t msg_idx);
static char* planPath(struct GridManager *gm);
static void recoverRobotAndStorage(struct GridManager *gm, size_t robot_i, size_t robot_j);


int readInputData(struct GridManager *gm, const char *file_name) {
    gm->free_robots = 0;
    
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

void createFifos(struct GridManager *gm, const char* output_fifo_path, const char* input_fifo_path) {
    gm->fifo_output_path = output_fifo_path;
    
    mkfifo(gm->fifo_output_path, 0666);
    gm->fd_output = open(gm->fifo_output_path, O_WRONLY);
    printf("[OUTPUT] FIFO opened\n");

    gm->fifo_input_path = input_fifo_path;
    mkfifo(gm->fifo_input_path, 0666);
    gm->fd_input = open(gm->fifo_input_path, O_RDONLY);
    printf("[INPUT] FIFO opened\n");
}

int writeInputData(struct GridManager *gm) {
    printf("Writing messages to [OUTPUT] FIFO:\n");

    printf("Grid size msg: %s", gm->msg_grid_size);
    write(gm->fd_output, gm->msg_grid_size, strlen(gm->msg_grid_size));

    printf("Storages points msg: %s", gm->msg_storages_points);
    write(gm->fd_output, gm->msg_storages_points, strlen(gm->msg_storages_points));

    printf("Items points msg: %s", gm->msg_items_points);
    write(gm->fd_output, gm->msg_items_points, strlen(gm->msg_items_points));

    printf("Robots points msg: %s", gm->msg_robots_points);
    write(gm->fd_output, gm->msg_robots_points, strlen(gm->msg_robots_points));

    printf("[OUTPUT] FIFO closed\n");
    close(gm->fd_output);

    return readConfirmation(gm);
}

static int readConfirmation(struct GridManager *gm) {
    printf("Reading messages from [INPUT] FIFO:\n");
    char buf[1024];
    read(gm->fd_input, &buf, 1024);
    printf("Read msg: %s\n", buf);

    if (strcmp(buf, "success") == 0) {
        return 1;
    } else {
        return -1;
    }
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
        gm->free_robots++;
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

static char* planPath(struct GridManager *gm) {
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

static void recoverRobotAndStorage(struct GridManager *gm, size_t robot_i, size_t robot_j) {
    gm->grid[robot_i][robot_j] = gm->free_robots + '0';
    gm->grid[robot_i + 1][robot_j] = 'S';
}

void printGrid(struct GridManager *gm) {
    for (size_t j=0; j<gm->grid_size_j; j++) {
        for (size_t i=0; i<gm->grid_size_i; i++) {
            printf("%c ", gm->grid[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int scanAndPlan(struct GridManager *gm) {
    const char* msg_path = planPath(gm);
    if (msg_path) {
        gm->free_robots--;
        printf("<- Sending messages to [OUTPUT] FIFO:\n");
        printf("Path msg: %s", msg_path);

        gm->fd_output = open(gm->fifo_output_path, O_WRONLY);
        write(gm->fd_output, msg_path, strlen(msg_path));
        close(gm->fd_output);

        free((void *)msg_path);

        printGrid(gm);

        return 1;
    } else {
        return 0;
    }
}

int readAndRecover(struct GridManager *gm) {
    char buf[1024];
    int status = read(gm->fd_input, &buf, 4);
    if (status != 0) {
        buf[4] = '\0';
        printf("-> Reading messages from [INPUT] FIFO:\n");
        printf("Robot finished: %s\n", buf);

        size_t robot_i = buf[0] - '0';
        size_t robot_j = buf[2] - '0';

        recoverRobotAndStorage(gm, robot_i, robot_j);
        gm->free_robots++;
        printGrid(gm);

        return 1;
    } else {
        return 0;
    }
}

void cleanUp(struct GridManager *gm) {
    close(gm->fd_input);
    printf("[INPUT] FIFO closed\n");

    for (size_t i=0; i<gm->grid_size_i; i++) {
        free(gm->grid[i]);
    }
    free(gm->grid);
}