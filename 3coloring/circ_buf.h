#ifndef C_EXERCISES_CIRC_BUF_H
#define C_EXERCISES_CIRC_BUF_H

struct circ_buf {
    graph_t buffer[MAX_GRAPH_COUNT];
    size_t readHead;
    size_t writeHead;
};

graph_t *writeToBuf();
graph_t *readFromBuf();

#endif //C_EXERCISES_CIRC_BUF_H