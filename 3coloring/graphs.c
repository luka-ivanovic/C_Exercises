#include "graphs.h"
#include <stddef.h>
#include <stdlib.h>
#include "common.h"

extern unsigned int colorIndex = 0;

graph_t *makeGraph(size_t vertexCount, size_t edgeCount){
    graph_t *graph = malloc(sizeof *graph);

    graph->n_edges = edgeCount;
    graph->n_vertices = vertexCount;

    graph->vertices = calloc(vertexCount, sizeof(vertex_t *));
    graph->edges = calloc(edgeCount, sizeof(edge_t *));
    return graph;
}

vertex_t *makeVertex(char Name) {
    vertex_t *vertex = malloc(sizeof *vertex);
    vertex->name = Name;
    vertex->color = colorIndex;
    colorIndex = (colorIndex + 1) % 3;
    return vertex;
}

edge_t *makeEdge(vertex_t *vertex1, vertex_t *vertex2) {
    edge_t *edge = malloc(sizeof *edge);
    edge->v1 = vertex1;
    edge->v2 = vertex2;
    return edge;
}

