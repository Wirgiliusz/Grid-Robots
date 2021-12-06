#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("Hello Controller!\n");

    FILE *file;
    file = fopen("input.txt", "r");
    if (!file) {
        perror("File opening failed");
        return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;
}