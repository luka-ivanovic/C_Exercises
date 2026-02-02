#ifndef C_EXERCISES_CIRC_BUF_H
#define C_EXERCISES_CIRC_BUF_H

struct circ_buf {
    size_t edgeCount;
    size_t readHead;
    size_t writeHead;
    edge_t *buffer[];
};

graph_t *writeToBuf();
graph_t *readFromBuf();

#endif //C_EXERCISES_CIRC_BUF_H