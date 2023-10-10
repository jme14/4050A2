#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cs4050.h"
#include "MST.h"

#define INF 1000000

int IsDirected(void){ // ??? 

    char input[100];
    int rando = 0;
    printf("The graph is going to be directed whether you like it or not!\n");
    if ( fgets(input, sizeof(input), stdin) != NULL){
        if (!sscanf(input, "%d", &rando)){
            printf("A problem occured\n");
        }
    }
    return 1;
}
void SetDirected(int fIsDirected){ // requires user input regarding if graph is directed or not  
    
}

void GetCounts(int * pCountVertices, int * pCountEdges){ // requires user input regarding amount of vertices and edges 

    char input[100];
    printf("Enter number of vertices: \n");
    if ( fgets(input, sizeof(input), stdin) != NULL){ // if fgets works
        if ( sscanf(input, "%d\n", pCountVertices) != 1){
            printf("Invalid number, start over\n");
            GetCounts(pCountVertices, pCountEdges);
            return;
        }
    } else {
        printf("Input too large, start over\n");
        printf("%s\n", input);
        GetCounts(pCountVertices, pCountEdges);
        return;
    }

    printf("Enter number of edges: \n");
    if ( fgets(input, sizeof(input), stdin) != NULL){ // if fgets works
        if ( sscanf(input, "%d", pCountEdges) != 1){
            printf("Invalid number, start over\n");
            GetCounts(pCountVertices, pCountEdges);
            return;
        }
    } else {
        printf("Input too large, start over\n");
        GetCounts(pCountVertices, pCountEdges);
        return;
    }

}

void GetEdge(Edge * pEdge){ // requires user input regarding edge content 

    char input[100];
    if ( fgets(input, sizeof(input), stdin) != NULL ){
        if ( !sscanf(input, "%d %d %f\n", &pEdge->from, &pEdge->to, &pEdge->weight)){
            printf("Invalid edge, try again\n");
            GetEdge(pEdge);
        }
    }
    else {
        printf("Input too large, try again\n");
        GetEdge(pEdge);
    }
}

void GetEdges(Edge edges[], int countEdges){ // requires user input of all edges 
    for ( int i = 0 ; i < countEdges ; i++){
        GetEdge(&edges[i]);
    }
}

void PrintVertex(Vertex vertex){ // prints information about the vertex 
    printf("Vertex %d:\n", vertex.number);

    Adjacency* adjLister = vertex.list;
    while ( adjLister != NULL ){

        int otherVertex = adjLister->pEdge->from;
        /* potentially remove the following */
        if ( otherVertex == vertex.number) otherVertex = adjLister->pEdge->to;
        /* */

        printf("\t%d->%d (%f)\n", vertex.number, otherVertex, adjLister->pEdge->weight);
        adjLister = adjLister->next;
    }
}

void PrintVertices(Vertex vertices[], int count){
    for ( int i = 0 ; i < count ; i++){
        PrintVertex(vertices[i]);
    }
}

void AddAdjacentEdge(Vertex * pVertex, Edge * pEdge){

    Adjacency* newAdjNode = malloc(sizeof(Adjacency));
    newAdjNode->next = NULL;
    newAdjNode->pEdge = pEdge;

    Adjacency* adjFinder = pVertex->list;
    if ( adjFinder == NULL ){
        pVertex->list = newAdjNode;
    } else {
        while ( adjFinder->next != NULL ) adjFinder = adjFinder->next;
        adjFinder->next = newAdjNode;
    }

}

void BuildAdjacency(Vertex vertices[], Edge edges[], int countVertices, int countEdges) {


    // vertices initialization
    for ( int i = 0 ; i < countVertices ; i++ ){
        vertices[i].number = i+1;
        vertices[i].color = WHITE;
        vertices[i].d = -1;
        vertices[i].f = -1;
        vertices[i].predecessor = -1;
    }

    // including data from each edge to each vertex containing an edge 
    for ( int i = 0 ; i < countEdges ; i++ ){
        int fromVertex = edges[i].from;
        AddAdjacentEdge(&vertices[fromVertex-1], &edges[i]);
    }
    
}

typedef struct _UnionFind{
    Vertex* vertex;
    int parent;
    int size;
} UnionFind;

void MakeSet(UnionFind* uf, Vertex* v){
    UnionFind* element = &uf[v->number-1];
    printf("Element points to %p\n", element);
    if ( element != 0 ) {
        element->parent = v->number-1;
        element->size = 1;
	element->vertex = v;
    }

}

int Find(UnionFind* uf, Vertex* v){

	int vIndex = (v->number)-1;
	printf("Looking at vIndex of %d\n", vIndex);
	
    if ( uf[vIndex].parent == vIndex) return vIndex;

    uf[vIndex].parent = Find(uf, uf[uf[vIndex].parent].vertex);
	printf("The parent of this is %d\n", uf[vIndex].parent);
    return uf[vIndex].parent;

}

void Union(UnionFind* uf, Vertex* u, Vertex* v){
    int uParent = Find(uf, u);
    int vParent = Find(uf, v);

    if ( uParent == vParent ) return;

    int parent = uParent;
    int nonParent = vParent;
    if ( uf[uParent].size < uf[vParent].size){
        parent = vParent;
        nonParent = uParent;
    }

    uf[nonParent].parent = parent;
    uf[parent].size += uf[nonParent].size;


}

Edge* sortEdgesByWeight(Edge edges[], int countEdges){

    // attempt mergesort 
    if ( countEdges <= 1 ) return edges;

    Edge* firstHalf = edges;
    Edge* secondHalf = &(edges[(countEdges/2)+1]);

    firstHalf = sortEdgesByWeight(firstHalf, countEdges/2 + countEdges%2);
    secondHalf = sortEdgesByWeight(firstHalf, countEdges/2);

    // merge 

    Edge* edgesSorted = malloc(sizeof(Edge)*countEdges);

    int firstInc = 0;
    int secondInc = 0;

    int i = 0;
    for (  ; i < countEdges && firstInc < countEdges/2 + countEdges%2 && secondInc < countEdges/2 ; i++ ){
        if ( firstHalf[firstInc].weight <= secondHalf[secondInc].weight){
            edgesSorted[i] = firstHalf[firstInc];
            firstInc++;
        } else {
            edgesSorted[i] = secondHalf[secondInc];
            secondInc++;
        }
    }

    while ( firstInc < countEdges/2 + countEdges%2){
        edgesSorted[i] = firstHalf[firstInc];
        firstInc++;
    }

    while ( secondInc < countEdges/2){
        edgesSorted[i] = secondHalf[secondInc];
        secondInc++;
    }

    return edgesSorted;
}

void MST_Kruskal(Vertex vertices[], int countVertices, Edge edges[], int countEdges)
{
	puts("MST:");
    // // make empty set A to hold edges 

    // for each vertex, 
    //      make set containing just that single vertex 

    UnionFind ufArray[countVertices];
    memset(ufArray, 0, sizeof(UnionFind));

    for ( int i = 0 ; i < countVertices ; i++ ){
        MakeSet(ufArray, &vertices[i]);
    }

    // sort list of edges by weight
    Edge* sortedEdges = sortEdgesByWeight(edges, countEdges);

    // for each edge e in ascending order 
    //      u = vert1 
    //      v = vert2 
    //      if ( vert1 not in same set as vert2 <union-find data structure> )
    // //            add e to A 
    //             UNION(u,v)
    //             print e to show addition into MST 

    for ( int i = 0 ; i < countEdges ; i++ ){

        Edge e = sortedEdges[i];

        Vertex* u = &vertices[ e.from-1 ];
        Vertex* v = &vertices[ e.to-1 ];
	

	int findResultU = Find(ufArray,u);
	int findResultV = Find(ufArray,v);
	printf("uFind: %d | vFind: %d\n", findResultU, findResultV);
        if ( Find(ufArray, u) != Find(ufArray, v)){
            Union(ufArray, u,v);
	    printf("\t%d -> %d (%4f)\n", e.from, e.to, e.weight);
	}
    }


}
