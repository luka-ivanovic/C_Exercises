#ifndef C_EXERCISES_SUPERVISOR_H
#define C_EXERCISES_SUPERVISOR_H

#include <semaphore.h>
#include "common.h"

typedef struct {
    sem_t *semFree;
    sem_t *semUsed;
    sem_t *semWriter;
} sems_t;

void printUsage(void);
int parseArgs(int argc, char **argv, unsigned long *limit, unsigned long *delay);
int setUpCircBuf(sems_t *sems);

#endif /* C_EXERCISES_SUPERVISOR_H */