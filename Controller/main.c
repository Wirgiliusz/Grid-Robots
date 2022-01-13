#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "GridManager.h"

#define INPUT_BUFFER_SIZE 1024

//void readAndAddStorages(FILE *file, char **grid);
//void readAndAddItems(FILE *file, char **grid);
//void readAndAddRobots(FILE *file, char **grid);
void printGrid(char **grid, size_t grid_size_i, size_t grid_size_j);


int main() {
    printf("Hello Controller!\n");

    struct GridManager gm;
    if(!openFile(&gm, "input.txt")) {
        perror("File opening failed");
        return EXIT_FAILURE;
    }

    readGridDimensions(&gm);


    readAndAddStorages(&gm);
    readAndAddItems(&gm);
    readAndAddRobots(&gm);

    printGrid(gm.grid, gm.grid_size_i, gm.grid_size_j);


    //int fd_input;
    int fd_output;
    //const char *fifo_input_path = "/tmp/myfifo_p2c";
    const char *fifo_output_path = "/tmp/myfifo_c2p";
    //mkfifo(fifo_input_path, 0666);
    mkfifo(fifo_output_path, 0666);
    // fd_input = open(fifo_input_path, O_RDONLY);
    fd_output = open(fifo_output_path, O_WRONLY);
    printf("FIFO opened\n");

    // TODO
    // Change the hard coded messeges to the data read from file
    // It is best to change implementation to struct based
    // GridMenager with all parameters and grid
    const char msg_grid_size[] = "6 6\n";
    const char msg_storages_points[] = "1 5 3 5 5 5\n";
    const char msg_items_points[] = "2 3 4 4\n";
    const char msg_robots_points[] = "0 0\n";
    write(fd_output, msg_grid_size, strlen(msg_grid_size));
    write(fd_output, msg_storages_points, strlen(msg_storages_points));
    write(fd_output, msg_items_points, strlen(msg_items_points));
    write(fd_output, msg_robots_points, strlen(msg_robots_points));

    close(fd_output);
    printf("FIFO closed\n");

    return EXIT_SUCCESS;
}

/*
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
*/

/*
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
*/

/*
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
*/

void printGrid(char **grid, size_t grid_size_i, size_t grid_size_j) {
    for (size_t j=0; j<grid_size_j; j++) {
        for (size_t i=0; i<grid_size_i; i++) {
            printf("%c ", grid[i][j]);
        }
        printf("\n");
    }
}