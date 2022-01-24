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
    if (readInputData(&gm, "input.txt") == -1) {
        perror("File opening failed");
        return EXIT_FAILURE;
    }

    gm.free_robots = 3;
    
    createFifos(&gm, "/tmp/myfifo_c2p", "/tmp/myfifo_p2c");
    if (writeInputData(&gm) == -1) {
        perror("Data connection failed");
        return EXIT_FAILURE;
    }
    
    while(1) {
        if (gm.free_robots) {
            scanAndPlan(&gm);
        }

        readAndRecover(&gm);
    }
    
    cleanUp(&gm);
    
    return EXIT_SUCCESS;
}
