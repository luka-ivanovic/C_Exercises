#ifndef GRAPHS_H
#define GRAPHS_H

#include <stddef.h>

typedef struct vertex {
    char name;
    int  color;
} vertex_t;

typedef struct edge {
    struct vertex *v1;
    struct vertex *v2;
} edge_t;

typedef struct graph {
    size_t n_vertices;
    size_t n_edges;
    size_t cap_vertices;
    size_t cap_edges;
    vertex_t **vertices;
    edge_t   **edges;
} graph_t;

graph_t *makeGraph(size_t vertexCap, size_t edgeCap);
vertex_t *makeVertex(char Name);
edge_t *makeEdge(vertex_t *vertex1, vertex_t *vertex2);

int containsVertex(graph_t *graph, char Name);
int addNewVertex(graph_t *graph, char Name);
edge_t *addNewEdge(graph_t *graph, vertex_t *v1, vertex_t *v2);
int cleanupGraph(graph_t *graph);

int removeEdgeSameColor(graph_t *graph, edge_t *removedEdges[]);
void removeEdgesToString(edge_t *removedEdges[], int removedEdgesCount);
void assignRandomColors(graph_t *graph);

#endif /* GRAPHS_H */