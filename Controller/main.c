#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "GridManager.h"


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

    printGrid(&gm);
    constructMessages(&gm);

    printf("Grid size msg: %s", gm.msg_grid_size);
    printf("Storages points msg: %s", gm.msg_storages_points);
    printf("Items points msg: %s", gm.msg_items_points);
    printf("Robots points msg: %s", gm.msg_robots_points);

    //int fd_input;
    int fd_output;
    //const char *fifo_input_path = "/tmp/myfifo_p2c";
    const char *fifo_output_path = "/tmp/myfifo_c2p";
    //mkfifo(fifo_input_path, 0666);
    mkfifo(fifo_output_path, 0666);
    // fd_input = open(fifo_input_path, O_RDONLY);
    fd_output = open(fifo_output_path, O_WRONLY);
    printf("FIFO opened\n");

    write(fd_output, gm.msg_grid_size, strlen(gm.msg_grid_size));
    write(fd_output, gm.msg_storages_points, strlen(gm.msg_storages_points));
    write(fd_output, gm.msg_items_points, strlen(gm.msg_items_points));
    write(fd_output, gm.msg_robots_points, strlen(gm.msg_robots_points));

    close(fd_output);
    printf("FIFO closed\n");

    return EXIT_SUCCESS;
}
