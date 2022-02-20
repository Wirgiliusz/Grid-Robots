#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "gridManager.h"
#include "pathPlanner.h"


enum States {ALL_ROBOTS_FREE, SOME_ROBOTS_FREE, ALL_ROBOTS_BUSY} state;
enum Events {ROBOT_ENGAGED, ROBOT_RELEASED};

enum States switchState(enum States current_state, enum Events event);

static struct GridManager gm;

int main(int argc, char **argv) {
    printf("Hello Controller!\n");
    state = ALL_ROBOTS_FREE;

    if (argc < 2) {
        printf("Missing argument with file name!\n");
        return EXIT_FAILURE;
    }

    char *input_file_name = argv[1];
    if (readInputData(&gm, input_file_name) == -1) {
        perror("File opening failed");
        return EXIT_FAILURE;
    }

    createFifos(&gm, "/tmp/myfifo_c2p", "/tmp/myfifo_p2c");
    if (writeInputData(&gm) == -1) {
        perror("Data connection failed");
        return EXIT_FAILURE;
    }
    
    while(1) {
        switch (state) {
            case ALL_ROBOTS_FREE:
                if (scanAndPlan(&gm)) {
                    state = switchState(state, ROBOT_ENGAGED);
                }
                break;
            case SOME_ROBOTS_FREE:
                if (scanAndPlan(&gm)) {
                    state = switchState(state, ROBOT_ENGAGED);
                }
                break;
            case ALL_ROBOTS_BUSY:
                if (readAndRecover(&gm)) {
                    state = switchState(state, ROBOT_RELEASED);
                }
                break;
        }
    }
    
    cleanUp(&gm);
    return EXIT_SUCCESS;
}

enum States switchState(enum States current_state, enum Events event) {
    switch (current_state) {
        case ALL_ROBOTS_FREE:
            if (event == ROBOT_ENGAGED) {
                return SOME_ROBOTS_FREE;
            } else if (event == ROBOT_RELEASED) {
                return current_state;
            }
            break;
        case SOME_ROBOTS_FREE:
            if (event == ROBOT_ENGAGED) {
                if (gm.free_robots == 0) {
                    return ALL_ROBOTS_BUSY;
                } else {
                    return current_state;
                }  
            } else if (event == ROBOT_RELEASED) {
                if (gm.free_robots == gm.max_robots) {
                    return ALL_ROBOTS_FREE;
                } else {
                    return current_state;
                }
            }
            break;
        case ALL_ROBOTS_BUSY:
            if (event == ROBOT_ENGAGED) {
                return current_state;
            } else if (event == ROBOT_RELEASED) {
                return SOME_ROBOTS_FREE;
            }
            break;
        default:
            return current_state;
    }   

    return current_state;
}