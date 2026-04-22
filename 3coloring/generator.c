#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include "common.h"
#include "circ_buf.h"

typedef struct {
    int32_t u;
    int32_t v;
} edge32_in_t;

typedef struct {
    int32_t *vertex_ids;
    size_t   n_vertices;
    edge32_in_t *edges;
    size_t   n_edges;
} graph_in_t;

static int parse_edge(const char *s, int32_t *u, int32_t *v) {
    char *end = NULL;
    long a = strtol(s, &end, 10);
    if (end == s || *end != '-') return -1;
    char *p = end + 1;
    long b = strtol(p, &end, 10);
    if (end == p || *end != '\0') return -1;
    if (a < 0 || b < 0) return -1;
    *u = (int32_t)a;
    *v = (int32_t)b;
    return 0;
}

static graph_in_t *build_graph(int argc, char **argv) {
    if (argc < 2) return NULL;

    edge32_in_t *edges = NULL;
    size_t cap = (size_t)(argc - 1);
    edges = (edge32_in_t *)malloc(cap * sizeof(edge32_in_t));
    if (!edges) return NULL;

    size_t m = 0;
    for (int i = 1; i < argc; i++) {
        int32_t u, v;
        if (parse_edge(argv[i], &u, &v) == 0) {
            edges[m].u = u;
            edges[m].v = v;
            m++;
        } else {
            fprintf(stderr, "Invalid edge: %s (expected A-B, non-negative ints)\n", argv[i]);
        }
    }
    if (m == 0) { free(edges); return NULL; }

    int32_t *verts = (int32_t *)malloc(m * 2 * sizeof(int32_t));
    if (!verts) { free(edges); return NULL; }
    size_t n = 0;
    for (size_t i = 0; i < m; i++) {
        int32_t a = edges[i].u, b = edges[i].v;
        int found = 0;
        for (size_t j = 0; j < n; j++) if (verts[j] == a) { found = 1; break; }
        if (!found) verts[n++] = a;
        found = 0;
        for (size_t j = 0; j < n; j++) if (verts[j] == b) { found = 1; break; }
        if (!found) verts[n++] = b;
    }

    graph_in_t *g = (graph_in_t *)malloc(sizeof(graph_in_t));
    if (!g) { free(edges); free(verts); return NULL; }
    g->vertex_ids = verts;
    g->n_vertices = n;
    g->edges = edges;
    g->n_edges = m;
    return g;
}

static void free_graph(graph_in_t *g) {
    if (!g) return;
    free(g->vertex_ids);
    free(g->edges);
    free(g);
}

static void generate_solution_colors(const graph_in_t *g, int *colors, solution_t *sol) {
    sol->count = 0;
    for (size_t i = 0; i < g->n_vertices; i++) {
        colors[i] = (rand() % 3) + 1;
    }

    for (size_t i = 0; i < g->n_edges; i++) {
        int32_t u = g->edges[i].u, v = g->edges[i].v;
        int cu = 0, cv = 0;
        for (size_t j = 0; j < g->n_vertices; j++) {
            if (g->vertex_ids[j] == u) cu = colors[j];
            if (g->vertex_ids[j] == v) cv = colors[j];
        }
        if (cu == cv) {
            if (sol->count < MAX_SOLUTION_EDGES) {
                sol->edges[sol->count].u = u;
                sol->edges[sol->count].v = v;
            }
            sol->count++;
        }
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s EDGE1 EDGE2 ... (e.g., 0-1 0-2 1-2)\n", argv[0]);
        return EXIT_FAILURE;
    }

    sem_t *semFree = sem_open(SEM_NAME_FREE, 0);
    if (semFree == SEM_FAILED) { perror("sem_open free"); return EXIT_FAILURE; }

    sem_t *semUsed = sem_open(SEM_NAME_USED, 0);
    if (semUsed == SEM_FAILED) { perror("sem_open used"); return EXIT_FAILURE; }

    sem_t *semWriter = sem_open(SEM_NAME_WRITER, 0);
    if (semWriter == SEM_FAILED) { perror("sem_open writer"); return EXIT_FAILURE; }

    circ_buf_t *buf = NULL;
    if (init_shm(&buf, 0) != 0) {
        fprintf(stderr, "init_shm attach failed\n");
        return EXIT_FAILURE;
    }

    graph_in_t *g = build_graph(argc, argv);
    if (!g) {
        fprintf(stderr, "Failed to build graph from arguments\n");
        return EXIT_FAILURE;
    }

    srand((unsigned)time(NULL) ^ (unsigned)getpid());

    int *colors = (int *)malloc(g->n_vertices * sizeof(int));
    if (!colors) { free_graph(g); return EXIT_FAILURE; }

    while (!buf->stop) {
        solution_t sol;
        generate_solution_colors(g, colors, &sol);

        if (sol.count > MAX_SOLUTION_EDGES) {
            continue;
        }

        if (sem_wait(semFree) == -1) {
            if (errno == EINTR) continue;
            perror("sem_wait free");
            break;
        }
        if (sem_wait(semWriter) == -1) {
            if (errno == EINTR) { sem_post(semFree); continue; }
            perror("sem_wait writer");
            sem_post(semFree);
            break;
        }

        if (buf->stop) {
            sem_post(semWriter);
            sem_post(semFree);
            break;
        }

        uint32_t idx = buf->writeHead;
        buf->buffer[idx] = sol;
        buf->writeHead = cbuf_next(idx);

        if (sem_post(semWriter) == -1) { perror("sem_post writer"); break; }
        if (sem_post(semUsed) == -1)   { perror("sem_post used");   break; }

        struct timespec ts = { .tv_sec = 0, .tv_nsec = 5 * 1000 * 1000 };
        nanosleep(&ts, NULL);
    }

    free(colors);
    free_graph(g);
    return 0;
}