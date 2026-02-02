#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "common.h"
#include "graphs.h"
#include "generator.h"

int main(int argc, char **argv) {
    int status = parseArgs(argc, argv);

    return EXIT_SUCCESS;

}

int parseArgs(int argc, char **argv) {
    int opt;
    while ((opt = getopt(argc, argv, "h")) != -1) {
        switch (opt) {
            case 'h':
                printf("Usage: %s [-h]\n",argv[0]);
                printf("Example\n\t generator 0-1 0-2 0-3 1-2 1-3 2-3");
                exit(EXIT_SUCCESS);
            default:
                printf("Usage: %s [-h]\n",argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    if (argc < 2) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

graph_t *setGraph(int argc, char **argv) {

}
