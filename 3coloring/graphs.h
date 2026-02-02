#ifndef C_EXERCISES_GRAPHS_H
#define C_EXERCISES_GRAPHS_H

#include <stddef.h>

#include "common.h"

vertex_t *makeVertex(char Name);
graph_t *makeGraph(size_t vertexCount, size_t edgeCount);
edge_t *makeEdge(vertex_t *vertex1, vertex_t *vertex2);
void removeEdgesToString(edge_t *removedEdges[], int removedEdgesCount);
int cleanupGraph(graph_t *graph);
int containsVertex(graph_t *graph, char Name);
int addNewVertex(graph_t *graph, char Name);
void assignRandomColors(graph_t *graph);
int removeEdgeSameColor(graph_t *graph, edge_t *removedEdges[]);

#endif //C_EXERCISES_GRAPHS_H