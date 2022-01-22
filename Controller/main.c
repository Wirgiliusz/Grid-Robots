#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "GridManager.h"

char* constructPath(int x_0, int y_0, int x_f, int y_f);
char* constructPathThroughPoint(int x_0, int y_0, int x_f, int y_f, int x_p, int y_p);
void addPathMessage(char *path, int x, int y, int msg_idx);

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

    
    printf("Reading messages from [INPUT] FIFO:\n");
    char buf[1024];
    read(fd_input, &buf, 1024);
    printf("Read msg: %s\n", buf);


    printf("Writing messages to [OUTPUT] FIFO:\n");
    const char *msg_path = constructPathThroughPoint(0, 0, 5, 5, 4, 4);
    printf("Movement path msg: %s", msg_path);
    write(fd_output, msg_path, strlen(msg_path));

    const char *msg_path2 = constructPath(3, 3, 1, 5);
    printf("Movement path msg: %s", msg_path2);
    write(fd_output, msg_path2, strlen(msg_path2));

    printf("Reading messages from [INPUT] FIFO:\n");
    while(1) {
        read(fd_input, &buf, 1024);
        if (strncmp(buf, "end", 3) == 0) {
            buf[3] = '\0';
            printf("Read msg: %s\n", buf);
            break;
        }
    }
    


    close(fd_output);
    printf("[OUTPUT] FIFO closed\n");

    close(fd_input);
    printf("[INPUT] FIFO closed\n");

    return EXIT_SUCCESS;
}


char* constructPath(int x_0, int y_0, int x_f, int y_f) {
    int x_t = x_0;
    int y_t = y_0;
    int i = 0;
    char *path = malloc(sizeof(char)*1024);
    addPathMessage(path, x_t, y_t, i);
    i += 4;

    if (x_t < x_f) {
        while (x_t < x_f) {
            ++x_t;
            addPathMessage(path, x_t, y_t, i);
            i += 4;
        }
    } else {
        while (x_t > x_f) {
            --x_t;
            addPathMessage(path, x_t, y_t, i);
            i += 4;
        }
    }
    if (y_t < y_f) {
        while (y_t < y_f) {
            ++y_t;
            addPathMessage(path, x_t, y_t, i);
            i += 4;
        }
    } else {
        while (y_t > y_f) {
            --y_t;
            addPathMessage(path, x_t, y_t, i);
            i += 4;
        }
    }

    path[i] = '\n';
    path[i + 1] = '\0';

    return path;
}

char* constructPathThroughPoint(int x_0, int y_0, int x_f, int y_f, int x_p, int y_p) {
    char *path_to_point = malloc(sizeof(char)*1024);
    char *path_from_point = malloc(sizeof(char)*1024);

    path_to_point = constructPath(x_0, y_0, x_p, y_p);
    size_t path_len = strlen(path_to_point);
    path_to_point[path_len - 5] = '\0';

    printf("Path to point: %s\n", path_to_point);

    path_from_point = constructPath(x_p, y_p, x_f, y_f);
    printf("Path from point: %s\n", path_from_point);

    strncat(path_to_point, path_from_point, path_len - 5);

    printf("Path connected: %s\n", path_to_point);

    return path_to_point;
}

void addPathMessage(char *path, int x, int y, int msg_idx) {
    path[msg_idx] = x + '0';
    path[msg_idx + 1] = ' ';
    path[msg_idx + 2] = y + '0';
    path[msg_idx + 3] = ' ';
}
