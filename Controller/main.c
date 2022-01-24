#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "gridManager.h"
#include "pathPlanner.h"


int main() {
    printf("Hello Controller!\n");

    struct GridManager gm;
    if(readInputData(&gm, "input.txt") == -1) {
        perror("File opening failed");
        return EXIT_FAILURE;
    }

    gm.free_robots = 3;
    
    gm.fifo_output_path = "/tmp/myfifo_c2p";
    
    mkfifo(gm.fifo_output_path, 0666);
    gm.fd_output = open(gm.fifo_output_path, O_WRONLY);
    printf("[OUTPUT] FIFO opened\n");

    gm.fifo_input_path = "/tmp/myfifo_p2c";
    mkfifo(gm.fifo_input_path, 0666);
    gm.fd_input = open(gm.fifo_input_path, O_RDONLY);
    printf("[INPUT] FIFO opened\n");
    
    
    printf("Writing messages to [OUTPUT] FIFO:\n");

    printf("Grid size msg: %s", gm.msg_grid_size);
    write(gm.fd_output, gm.msg_grid_size, strlen(gm.msg_grid_size));

    printf("Storages points msg: %s", gm.msg_storages_points);
    write(gm.fd_output, gm.msg_storages_points, strlen(gm.msg_storages_points));

    printf("Items points msg: %s", gm.msg_items_points);
    write(gm.fd_output, gm.msg_items_points, strlen(gm.msg_items_points));

    printf("Robots points msg: %s", gm.msg_robots_points);
    write(gm.fd_output, gm.msg_robots_points, strlen(gm.msg_robots_points));

    close(gm.fd_output);

    printf("Reading messages from [INPUT] FIFO:\n");
    char buf[1024];
    read(gm.fd_input, &buf, 1024);
    printf("Read msg: %s\n", buf);

    while(1) {
        if (gm.free_robots) {
            scanAndPlan(&gm);
        }

        int status = read(gm.fd_input, &buf, 4);
        if (status != 0) {
            buf[4] = '\0';
            printf("Read msg: %s\n", buf);

            size_t robot_i = buf[0] - '0';
            size_t robot_j = buf[2] - '0';

            recoverRobotAndStorage(&gm, robot_i, robot_j);
            gm.free_robots++;
            printGrid(&gm);
        }
    }
    
    
    printf("[OUTPUT] FIFO closed\n");

    close(gm.fd_input);
    printf("[INPUT] FIFO closed\n");

    return EXIT_SUCCESS;
}
