#ifndef C_EXERCISES_GRAPHS_H
#define C_EXERCISES_GRAPHS_H

#include <stddef.h>

#include "common.h"

vertex_t *makeVertex(char Name);
graph_t *makeGraph(size_t vertexCount, size_t edgeCount);
edge_t *makeEdge(vertex_t *vertex1, vertex_t *vertex2);



#endif //C_EXERCISES_GRAPHS_H