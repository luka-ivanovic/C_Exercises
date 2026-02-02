#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <errno.h>
#include "common.h"
#include "supervisor.h"

int main(int argc, char **argv) {

    unsigned long  limit = 0;
    unsigned long delay = 0;
    int boolean = 0;
    sems_t allSems = {
        NULL,
        NULL,
        NULL
    };

    if (parseArgs(argc, argv, &limit, &delay) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    if (setUpCircBuf(&allSems) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    if (limit == 0) boolean = 1;
    while (boolean || limit > 0) {
        //read from shm
        sem_wait(allSems.semUsed);


        sem_post(allSems.semFree);
        if (limit > 0) limit--;
    }

    return EXIT_SUCCESS;
}

void printUsage(void) {
    printf("supervisor [-n limit] [-w delay]");
}

int parseArgs(int argc, char **argv, unsigned long *limit, unsigned long *delay) {
    int opt;
    while ((opt = getopt(argc, argv, "n:w:"))!= -1) {
        switch (opt) {
            case 'n':
                errno = 0;
                *limit = (unsigned) strtol(optarg, NULL, 10);
                if (errno != 0) {
                    perror("Invalid delay");
                    return EXIT_FAILURE;
                }
                break;
            case 'w':
                errno = 0;
                *delay = (unsigned) strtol(optarg, NULL, 10);
                if (errno != 0) {
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

int setUpCircBuf(sems_t *allSems) {
    allSems->semFree = sem_open(SEM_NAME_FREE, O_CREAT | O_EXCL, SEM_MODE, MAX_GRAPH_COUNT);
    if (allSems->semFree == SEM_FAILED) {
        perror("Failed to create semaphore");
        goto fail;
    }

    allSems->semUsed = sem_open(SEM_NAME_USED, O_CREAT |O_EXCL, SEM_MODE, 0);
    if (allSems->semUsed == SEM_FAILED) {
        perror("Failed to create semaphore");
        goto fail;
    }

    allSems->semGenerator = sem_open(SEM_NAME_GENERATOR, O_CREAT |O_EXCL, SEM_MODE, 1);
    if (allSems->semGenerator == SEM_FAILED) {
        perror("Failed to create semaphore");
        goto fail;
    }
    return EXIT_SUCCESS;

    fail:
      if (allSems->semUsed != SEM_FAILED) {
          sem_close(allSems->semUsed);
          sem_unlink(SEM_NAME_USED);
      }
      if (allSems->semGenerator != SEM_FAILED) {
        sem_close(allSems->semGenerator);
        sem_unlink(SEM_NAME_GENERATOR);
      }
      if (allSems->semFree != SEM_FAILED) {
        sem_close(allSems->semFree);
        sem_unlink(SEM_NAME_FREE);
      }
      return EXIT_FAILURE;
}