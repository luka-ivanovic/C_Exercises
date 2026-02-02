#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "common.h"
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
        }
    }
    return EXIT_SUCCESS;
}