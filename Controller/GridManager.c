#include "GridManager.h"

int openFile(struct GridManager *gm, const char *file_name) {
    gm->file = fopen(file_name, "r");
    if (!gm->file) {
        perror("File opening failed");
        return 0;
    } else {
        return 1;
    }
}