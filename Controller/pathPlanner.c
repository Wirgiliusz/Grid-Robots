#include "pathPlanner.h"
#include <stdlib.h>
#include <string.h>

char* constructPath(size_t x_0, size_t y_0, size_t x_f, size_t y_f) {
    size_t x_t = x_0;
    size_t y_t = y_0;
    size_t i = 0;
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

char* constructPathThroughPoint(size_t x_0, size_t y_0, size_t x_f, size_t y_f, size_t x_p, size_t y_p) {
    char *path_to_point = malloc(sizeof(char)*1024);
    char *path_from_point = malloc(sizeof(char)*1024);

    path_to_point = constructPath(x_0, y_0, x_p, y_p);
    size_t path_len = strlen(path_to_point);
    path_to_point[path_len - 5] = '\0';

    printf("Path to point: %s\n", path_to_point);

    path_from_point = constructPath(x_p, y_p, x_f, y_f);
    printf("Path from point: %s\n", path_from_point);

    strncat(path_to_point, path_from_point, strlen(path_from_point));

    printf("Path connected: %s\n", path_to_point);

    return path_to_point;
}

void addPathMessage(char *path, size_t x, size_t y, size_t msg_idx) {
    path[msg_idx] = x + '0';
    path[msg_idx + 1] = ' ';
    path[msg_idx + 2] = y + '0';
    path[msg_idx + 3] = ' ';
}