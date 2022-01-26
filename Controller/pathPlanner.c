#include "pathPlanner.h"
#include <stdlib.h>
#include <string.h>

static char* constructPath(size_t x_0, size_t y_0, size_t x_f, size_t y_f);
static void addPathMessage(char *path, size_t x, size_t y, size_t msg_idx);

static char* constructPath(size_t x_0, size_t y_0, size_t x_f, size_t y_f) {
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

static void addPathMessage(char *path, size_t x, size_t y, size_t msg_idx) {
    path[msg_idx] = x + '0';
    path[msg_idx + 1] = ' ';
    path[msg_idx + 2] = y + '0';
    path[msg_idx + 3] = ' ';
}

char* constructPathThroughPoint(size_t x_0, size_t y_0, size_t x_f, size_t y_f, size_t x_p, size_t y_p) {
    char *path_start;
    char *path_to_point;
    char *path_from_point;

    path_start = constructPath(x_0, y_0, x_0, y_0 - 1);
    path_start[strlen(path_start) - 5] = '\0';

    path_to_point = constructPath(x_0, y_0 - 1, x_p, y_p);
    path_to_point[strlen(path_to_point) - 5] = '\0';

    strncat(path_start, path_to_point, strlen(path_to_point));

    path_from_point = constructPath(x_p, y_p, x_f, y_f);
    size_t path_len = strlen(path_from_point);
    addPathMessage(path_from_point, x_f - 1, y_f, path_len - 1);
    path_from_point[path_len + 3] = '\n';
    path_from_point[path_len + 4] = '\0';

    strncat(path_start, path_from_point, strlen(path_from_point));

    free(path_to_point);
    free(path_from_point);
    return path_start;
}
