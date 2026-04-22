#ifndef CIRC_BUF_H
#define CIRC_BUF_H

#include <stdint.h>
#include <stddef.h>
#include "common.h"

typedef struct {
    int32_t u;
    int32_t v;
} edge32_t;

typedef struct {
    uint32_t count;
    edge32_t edges[MAX_SOLUTION_EDGES];
} solution_t;

typedef struct {
    volatile uint32_t stop;
    uint32_t readHead;
    uint32_t writeHead;
    solution_t buffer[CBUF_CAPACITY];
} circ_buf_t;

int init_shm(circ_buf_t **buf, int create);

static inline uint32_t cbuf_next(uint32_t x) { return (uint32_t)((x + 1) % CBUF_CAPACITY); }

#endif /* CIRC_BUF_H */