#include "graphs.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "common.h"

graph_t *makeGraph(size_t vertexCount, size_t edgeCount){
    graph_t *graph = malloc(sizeof *graph);
    if (!graph) return NULL;

    graph->n_edges = edgeCount;
    graph->n_vertices = vertexCount;

    graph->vertices = calloc(vertexCount, sizeof(vertex_t *));
    graph->edges = calloc(edgeCount, sizeof(edge_t *));
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
    if (containsVertex(graph, Name))
        return 0;

    for (size_t i = 0; i < graph->n_vertices; i++) {
        if (graph->vertices[i] == NULL) {
            graph->vertices[i] = makeVertex(Name);
            return 1;
        }
    }
    return 0;
}

int cleanupGraph(graph_t *graph) {
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
    int removedEdgesCount = 0;
    for (size_t i = 0; i < graph->n_edges; i++) {
        edge_t *e = graph->edges[i];
        if (!e || !e->v1 || !e->v2) {
            continue;
        }
        if (e->v1->color == e->v2->color) {
            removedEdges[removedEdgesCount++] = e;
        }
    }
    return removedEdgesCount;
}

//Used for the supervisor
void removeEdgesToString(edge_t *removedEdges[], int removedEdgesCount) {
    if (removedEdgesCount > 0) {
        printf("Solution with %i edges: ", removedEdgesCount);
    }else {
        printf("The graph is 3-colorable!");
        return;
    }
    for (size_t i = 0; i < removedEdgesCount; i++) {
        printf("%c-%c", removedEdges[i]->v1->name, removedEdges[i]->v2->name);
        if (i != removedEdgesCount - 1) {
            printf(" ");
        }
    }
    printf("\n");
}

void assignRandomColors(graph_t *graph) {
    srand(time(NULL));
    for (size_t i = 0; i < graph->n_vertices; i++) {
        if (graph->vertices[i])
            graph->vertices[i]->color = (rand() % 3) + 1;
    }
}