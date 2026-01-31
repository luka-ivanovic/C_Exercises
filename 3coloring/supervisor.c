#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "common.h"

unsigned limit = 0;
unsigned delay = 0;

void printUsage(void);
int parseArgs(int argc, char **argv);
int setUpCircBuf(void);

int main(int argc, char **argv) {
    if (parseArgs(argc, argv) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    return 0;
}

void printUsage(void) {
    printf("supervisor [-n limit] [-w delay]");
}

int parseArgs(int argc, char **argv) {
    int opt;
    while ((opt = getopt(argc, argv, "n:w:"))!= -1) {
        switch (opt) {
            case 'n':
                limit = strtol(optarg, NULL, 10);
                if (limit == LONG_MAX || limit == LONG_MIN) {
                    perror("Invalid delay");
                    return EXIT_FAILURE;
                }
                break;
            case 'w':
                delay = strtol(optarg, NULL, 10);
                if (delay == LONG_MAX || delay == LONG_MIN) {
                    perror("Invalid delay");
                    return EXIT_FAILURE;
                }
                break;
            default:
                printUsage();
                return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}