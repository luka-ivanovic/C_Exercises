#ifndef C_EXERCISES_SUPERVISOR_H
#define C_EXERCISES_SUPERVISOR_H

typedef struct {
    sem_t *semFree;
    sem_t *semUsed;
    sem_t *semGenerator;
} sems_t;

void printUsage(void);
int parseArgs(int argc, char **argv, unsigned long *limit, unsigned long *delay);
int setUpCircBuf(sems_t *sems);

#endif //C_EXERCISES_SUPERVISOR_H