#ifndef C_EXERCISES_COMMON_H
#define C_EXERCISES_COMMON_H

static const char *SEM_NAME_USED = "SEM_NAME_USED";
static const char *SEM_NAME_FREE = "SEM_NAME_FREE";
static const char *SEM_NAME_GENERATOR = "SEM_NAME_GENERATOR";
static const char *SHM_NAME = "SHM_NAME";
static const mode_t SEM_MODE = 0666;

static const int MAX_CIRC_BUF_MEM = 4096;
enum color {COLOR_RED, COLOR_GREEN, COLOR_BLUE};

typedef struct {
    char name;
    enum color color;
}vertex_t;

typedef struct {
    vertex_t *v1;
    vertex_t *v2;
}edge_t;

typedef struct {
    vertex_t **vertices;
    size_t n_vertices;

    size_t n_edges;
    edge_t **edges;
}graph_t;

#endif