#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "gridManager.h"
#include "pathPlanner.h"


enum States {ALL_ROBOTS_FREE, TWO_ROBOTS_FREE, ONE_ROBOT_FREE, ALL_ROBOTS_BUSY} state;
enum Events {ROBOT_ENGAGED, ROBOT_RELEASED};

enum States switchState(enum States current_state, enum Events event);

int main(int argc, char **argv) {
    printf("Hello Controller!\n");
    state = ALL_ROBOTS_FREE;

    if (argc < 2) {
        printf("Missing argument with file name!\n");
        return EXIT_FAILURE;
    }

    char *input_file_name = argv[1];
    struct GridManager gm;
    if (readInputData(&gm, input_file_name) == -1) {
        perror("File opening failed");
        return EXIT_FAILURE;
    }

    createFifos(&gm, "/tmp/myfifo_c2p", "/tmp/myfifo_p2c");
    if (writeInputData(&gm) == -1) {
        perror("Data connection failed");
        return EXIT_FAILURE;
    }
    
    int robot_engaged = 0;
    int robot_released = 0;
    while(1) {
        if (gm.free_robots) {
            robot_engaged = scanAndPlan(&gm);
            if (robot_engaged) {
                switchState(state, ROBOT_ENGAGED);
            }
        }

        robot_released = readAndRecover(&gm);
        if (robot_released) {
            switchState(state, ROBOT_RELEASED);
        }
    }
    
    cleanUp(&gm);
    
    return EXIT_SUCCESS;
}

enum States switchState(enum States current_state, enum Events event) {
    switch (current_state) {
        case ALL_ROBOTS_FREE:
            if (event == ROBOT_ENGAGED) {
                return TWO_ROBOTS_FREE;
            } else if (event == ROBOT_RELEASED) {
                return event;
            }
            break;
        case TWO_ROBOTS_FREE:
            if (event == ROBOT_ENGAGED) {
                return ONE_ROBOT_FREE;
            } else if (event == ROBOT_RELEASED) {
                return ALL_ROBOTS_FREE;
            }
            break;
        case ONE_ROBOT_FREE:
            if (event == ROBOT_ENGAGED) {
                return ALL_ROBOTS_BUSY;
            } else if (event == ROBOT_RELEASED) {
                return TWO_ROBOTS_FREE;
            }
            break;
        case ALL_ROBOTS_BUSY:
            if (event == ROBOT_ENGAGED) {
                return event;
            } else if (event == ROBOT_RELEASED) {
                return ONE_ROBOT_FREE;
            }
            break;
        default:
            return current_state;
    }   

    return event;
}