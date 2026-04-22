#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/mman.h>
#include "common.h"
#include "circ_buf.h"
#include "supervisor.h"

static volatile sig_atomic_t g_stop = 0;
static circ_buf_t *g_buf = NULL;

static void on_signal(int sig) {
    (void)sig;
    g_stop = 1;
}

static void print_solution_stderr(const solution_t *sol) {
    if (!sol) return;
    if (sol->count == 0) {
        fprintf(stderr, "New best: 0 edges\n");
        return;
    }
    fprintf(stderr, "New best: %u edges: ", sol->count);
    for (uint32_t i = 0; i < sol->count; i++) {
        fprintf(stderr, "%d-%d", sol->edges[i].u, sol->edges[i].v);
        if (i + 1 != sol->count) fprintf(stderr, ", ");
    }
    fprintf(stderr, "\n");
}

void printUsage(void) {
    fprintf(stderr, "Usage: supervisor [-n limit] [-w delay]\n");
    fprintf(stderr, "  -n limit: max number of solutions to read (0 = infinite)\n");
    fprintf(stderr, "  -w delay: delay in seconds before reading first solution\n");
}

int parseArgs(int argc, char **argv, unsigned long *limit, unsigned long *delay) {
    int opt;
    char *endptr;
    opterr = 0;
    optind = 1;
    *limit = 0;
    *delay = 0;

    while ((opt = getopt(argc, argv, "n:w:")) != -1) {
        switch (opt) {
        case 'n':
            errno = 0;
            *limit = strtoul(optarg, &endptr, 10);
            if (errno || *endptr != '\0') return EXIT_FAILURE;
            break;
        case 'w':
            errno = 0;
            *delay = strtoul(optarg, &endptr, 10);
            if (errno || *endptr != '\0') return EXIT_FAILURE;
            break;
        default:
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

int setUpCircBuf(sems_t *sems) {
    if (!sems) { errno = EINVAL; return -1; }

    sem_unlink(SEM_NAME_FREE);
    sem_unlink(SEM_NAME_USED);
    sem_unlink(SEM_NAME_WRITER);
    shm_unlink(SHM_NAME);

    sems->semFree = sem_open(SEM_NAME_FREE, O_CREAT, 0666, CBUF_CAPACITY);
    if (sems->semFree == SEM_FAILED) { perror("sem_open free"); return -1; }

    sems->semUsed = sem_open(SEM_NAME_USED, O_CREAT, 0666, 0);
    if (sems->semUsed == SEM_FAILED) { perror("sem_open used"); return -1; }

    sems->semWriter = sem_open(SEM_NAME_WRITER, O_CREAT, 0666, 1);
    if (sems->semWriter == SEM_FAILED) { perror("sem_open writer"); return -1; }

    if (init_shm(&g_buf, 1) != 0) {
        fprintf(stderr, "init_shm failed\n");
        return -1;
    }
    return 0;
}

int main(int argc, char **argv) {
    unsigned long limit = 0, delay = 0;
    if (parseArgs(argc, argv, &limit, &delay) != EXIT_SUCCESS) {
        printUsage();
        return EXIT_FAILURE;
    }

    sems_t sems;
    if (setUpCircBuf(&sems) != 0) return EXIT_FAILURE;

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = on_signal;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    if (delay > 0) sleep((unsigned int)delay);

    solution_t best;
    best.count = UINT32_MAX;
    unsigned long read_count = 0;

    while (!g_stop) {
        if (limit > 0 && read_count >= limit) break;

        if (sem_wait(sems.semUsed) == -1) {
            if (errno == EINTR) break;
            perror("sem_wait used");
            break;
        }

        uint32_t idx = g_buf->readHead;
        solution_t sol = g_buf->buffer[idx];
        g_buf->readHead = cbuf_next(idx);

        if (sem_post(sems.semFree) == -1) { perror("sem_post free"); break; }

        read_count++;

        if (sol.count == 0) {

            fprintf(stdout, "The graph is 3-colorable!\n");
            fflush(stdout);
            break;
        }

        if (sol.count < best.count) {
            best = sol;
            print_solution_stderr(&best);
        }
    }

    g_buf->stop = 1;

    for (int i = 0; i < 8; i++) {
        sem_post(sems.semFree);
        sem_post(sems.semUsed);
        sem_post(sems.semWriter);
    }

    if (best.count != 0) {
        if (!(g_stop == 0 && read_count == 0 && best.count == UINT32_MAX)) {

            if (limit > 0 && read_count >= limit) {
                fprintf(stdout,
                        "The graph might not be 3-colorable, best solution removes %u edges.\n",
                        (best.count == UINT32_MAX ? 0u : best.count));
            } else if (g_stop) {
                fprintf(stdout,
                        "The graph might not be 3-colorable, best solution removes %u edges.\n",
                        (best.count == UINT32_MAX ? 0u : best.count));
            }
        }
        fflush(stdout);
    }

    sem_unlink(SEM_NAME_FREE);
    sem_unlink(SEM_NAME_USED);
    sem_unlink(SEM_NAME_WRITER);
    shm_unlink(SHM_NAME);
    return 0;
}