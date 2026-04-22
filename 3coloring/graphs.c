#define _POSIX_C_SOURCE 200809L
#include "graphs.h"
#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

graph_t *makeGraph(size_t vertexCap, size_t edgeCap){
    graph_t *graph = malloc(sizeof *graph);
    if (!graph) return NULL;

    graph->n_vertices = 0;
    graph->n_edges = 0;
    graph->cap_vertices = vertexCap;
    graph->cap_edges = edgeCap;

    graph->vertices = calloc(vertexCap, sizeof(vertex_t *));
    graph->edges    = calloc(edgeCap, sizeof(edge_t *));
    if (!graph->vertices || !graph->edges) {
        free(graph->vertices);
        free(graph->edges);
        free(graph);
        return NULL;
    }
    return graph;
}

vertex_t *makeVertex(char Name) {
    vertex_t *vertex = malloc(sizeof *vertex);
    if (!vertex) return NULL;
    vertex->name = Name;
    vertex->color = 0;
    return vertex;
}

edge_t *makeEdge(vertex_t *vertex1, vertex_t *vertex2) {
    edge_t *edge = malloc(sizeof *edge);
    if (!edge) return NULL;
    edge->v1 = vertex1;
    edge->v2 = vertex2;
    return edge;
}

int containsVertex(graph_t *graph, char Name) {
    for (size_t i = 0; i < graph->n_vertices; i++) {
        if (graph->vertices[i] && Name == graph->vertices[i]->name) {
            return 1;
        }
    }
    return 0;
}

int addNewVertex(graph_t *graph, char Name) {
    if (containsVertex(graph, Name)) return 0;
    if (graph->n_vertices >= graph->cap_vertices) return 0;
    vertex_t *v = makeVertex(Name);
    if (!v) return 0;
    graph->vertices[graph->n_vertices++] = v;
    return 1;
}

edge_t *addNewEdge(graph_t *graph, vertex_t *v1, vertex_t *v2) {
    if (graph->n_edges >= graph->cap_edges) return NULL;
    edge_t *e = makeEdge(v1, v2);
    if (!e) return NULL;
    graph->edges[graph->n_edges++] = e;
    return e;
}

int cleanupGraph(graph_t *graph) {
    if (!graph) return 0;
    for (size_t i = 0; i < graph->n_vertices; i++) {
        free(graph->vertices[i]);
    }
    for (size_t i = 0; i < graph->n_edges; i++) {
        free(graph->edges[i]);
    }
    free(graph->vertices);
    free(graph->edges);
    free(graph);
    return 0;
}

int removeEdgeSameColor(graph_t *graph, edge_t *removedEdges[]) {
    int count = 0;
    for (size_t i = 0; i < graph->n_edges; i++) {
        if (graph->edges[i]->v1->color == graph->edges[i]->v2->color) {
            removedEdges[count++] = graph->edges[i];
        }
    }
    return count;
}

void removeEdgesToString(edge_t *removedEdges[], int removedEdgesCount) {
    if (removedEdgesCount > 0) {
        printf("Solution with %i edges: ", removedEdgesCount);
    } else {
        printf("The graph is 3-colorable!");
        printf("\n");
        return;
    }
    for (size_t i = 0; i < (size_t)removedEdgesCount; i++) {
        printf("%c-%c", removedEdges[i]->v1->name, removedEdges[i]->v2->name);
        if (i + 1 != (size_t)removedEdgesCount) {
            printf(", ");
        }
    }
    printf("\n");
}

void assignRandomColors(graph_t *graph) {
    for (size_t i = 0; i < graph->n_vertices; i++) {
        graph->vertices[i]->color = (rand() % 3) + 1;
    }
}