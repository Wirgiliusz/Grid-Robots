#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "gridManager.h"
#include "pathPlanner.h"

size_t free_robots = 3;

int main() {
    printf("Hello Controller!\n");

    struct GridManager gm;
    if(readInputData(&gm, "input.txt") == -1) {
        perror("File opening failed");
        return EXIT_FAILURE;
    }

    int fd_input;
    int fd_output;
    
    const char *fifo_output_path = "/tmp/myfifo_c2p";
    mkfifo(fifo_output_path, 0666);
    fd_output = open(fifo_output_path, O_WRONLY);
    printf("[OUTPUT] FIFO opened\n");

    const char *fifo_input_path = "/tmp/myfifo_p2c";
    mkfifo(fifo_input_path, 0666);
    fd_input = open(fifo_input_path, O_RDONLY);
    printf("[INPUT] FIFO opened\n");
    
    
    printf("Writing messages to [OUTPUT] FIFO:\n");

    printf("Grid size msg: %s", gm.msg_grid_size);
    write(fd_output, gm.msg_grid_size, strlen(gm.msg_grid_size));

    printf("Storages points msg: %s", gm.msg_storages_points);
    write(fd_output, gm.msg_storages_points, strlen(gm.msg_storages_points));

    printf("Items points msg: %s", gm.msg_items_points);
    write(fd_output, gm.msg_items_points, strlen(gm.msg_items_points));

    printf("Robots points msg: %s", gm.msg_robots_points);
    write(fd_output, gm.msg_robots_points, strlen(gm.msg_robots_points));

    close(fd_output);

    printf("Reading messages from [INPUT] FIFO:\n");
    char buf[1024];
    read(fd_input, &buf, 1024);
    printf("Read msg: %s\n", buf);

    while(1) {
        if (free_robots) {
            const char* msg_path = planPath(&gm);
            if (msg_path) {
                free_robots--;
                printf("Movement path msg: %s", msg_path);

                fd_output = open(fifo_output_path, O_WRONLY);
                write(fd_output, msg_path, strlen(msg_path));
                close(fd_output);

                free((void *)msg_path);

                printGrid(&gm);
            }
        } 

        int status = read(fd_input, &buf, 4);
        if (status != 0) {
            buf[4] = '\0';
            printf("Read msg: %s\n", buf);

            size_t robot_i = buf[0] - '0';
            size_t robot_j = buf[2] - '0';

            recoverRobotAndStorage(&gm, robot_i, robot_j);
            free_robots++;
            printGrid(&gm);
        }
    }
    
    
    printf("[OUTPUT] FIFO closed\n");

    close(fd_input);
    printf("[INPUT] FIFO closed\n");

    return EXIT_SUCCESS;
}
