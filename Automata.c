#include <stdio.h>
#include <stdlib.h>

/*
Algorithm:

- needs a reader that scan every character.
- needs a state machine (graph).
*/

struct Graph {
    int numberOfNodes;
    char** edges;
};

void initGraph(struct Graph graph, int numberOfNodes);

int main() {

    struct Graph graph;
    initGraph(graph, 10);

    graph.edges[1] = "victor\0";
    printf("%c", graph.edges[1][0]);
    printf("%c", graph.edges[1][1]);
    printf("%c", graph.edges[1][2]);
    printf("%c", graph.edges[1][3]);
    printf("%c", graph.edges[1][4]);
    printf("%c", graph.edges[1][5]);
    
    // graph.edges[1] = "victor\0";
    // printf("%s", graph.edges[1]);

    return 0;
}


void initGraph(struct Graph graph, int numberOfNodes) {
    graph.numberOfNodes = numberOfNodes;

    int i,j;
    for(i = 0; i < numberOfNodes; i++)
        graph.edges[i] = malloc(sizeof(char[numberOfNodes]));
}