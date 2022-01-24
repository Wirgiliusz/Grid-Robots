#include <stdio.h>

#define INPUT_BUFFER_SIZE 1024

char* constructPath(size_t x_0, size_t y_0, size_t x_f, size_t y_f);
char* constructPathThroughPoint(size_t x_0, size_t y_0, size_t x_f, size_t y_f, size_t x_p, size_t y_p);
void addPathMessage(char *path, size_t x, size_t y, size_t msg_idx);
