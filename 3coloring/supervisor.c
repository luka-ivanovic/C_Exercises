#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include "common.h"
#include "supervisor.h"

unsigned limit = 0;
unsigned delay = 0;
sem_t *semFree;
sem_t *semUsed;
sem_t *semGenerator;

int main(int argc, char **argv) {
    if (parseArgs(argc, argv) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    if (setUpCircBuf() == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
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

int setUpCircBuf(void) {
    semFree = sem_open(SEM_NAME_FREE, O_CREAT |O_EXCL, SEM_MODE, MAX_CIRC_BUF_MEM);
    if (semFree == SEM_FAILED) {
        perror("Failed to create semaphore");
        goto fail;
    }

    semUsed = sem_open(SEM_NAME_USED, O_CREAT |O_EXCL, SEM_MODE, 0);
    if (semUsed == SEM_FAILED) {
        perror("Failed to create semaphore");
        goto fail;
    }

    semGenerator = sem_open(SEM_NAME_GENERATOR, O_CREAT |O_EXCL, SEM_MODE, 1);
    if (semGenerator == SEM_FAILED) {
        perror("Failed to create semaphore");
        goto fail;
    }
    return EXIT_SUCCESS;

    fail:
      if (semUsed != SEM_FAILED) {
          sem_close(semUsed);
          sem_unlink(SEM_NAME_USED);
      }
      if (semGenerator != SEM_FAILED) {
        sem_close(semGenerator);
        sem_unlink(SEM_NAME_GENERATOR);
      }
      if (semFree != SEM_FAILED) {
        sem_close(semFree);
        sem_unlink(SEM_NAME_FREE);
      }
      return EXIT_FAILURE;
}