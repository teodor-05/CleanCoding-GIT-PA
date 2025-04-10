#include <stdlib.h>
#include <stdio.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct Graph {
    int vertices;
    int* visited;
    Node** adjacency_lists;
} Graph;

// Utility Functions
Node* create_node(int value) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = value;
    new_node->next = NULL;
    return new_node;
}

Graph* create_graph(int vertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->vertices = vertices;
    graph->adjacency_lists = (Node**)malloc(vertices * sizeof(Node*));
    graph->visited = (int*)malloc(vertices * sizeof(int));

    for (int i = 0; i < vertices; i++) {
        graph->adjacency_lists[i] = NULL;
        graph->visited[i] = 0;
    }
    return graph;
}

void add_edge(Graph* graph, int src, int dest) {
    Node* new_node = create_node(dest);
    new_node->next = graph->adjacency_lists[src];
    graph->adjacency_lists[src] = new_node;

    new_node = create_node(src);
    new_node->next = graph->adjacency_lists[dest];
    graph->adjacency_lists[dest] = new_node;
}

void insert_edges(int vertices, int edges, Graph* graph) {
    printf("Add %d edges (from 0 to %d)\n", edges, vertices - 1);
    for (int i = 0; i < edges; i++) {
        int src, dest;
        scanf("%d %d", &src, &dest);
        add_edge(graph, src, dest);
    }
}

// BFS Utility Functions
int is_empty(Node* queue) {
    return queue == NULL;
}

void enqueue(Node** queue, int data) {
    Node* new_node = create_node(data);
    
    if (is_empty(*queue)) {
        *queue = new_node;
    } else {
        Node* temp = *queue;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = new_node;
    }
}

int dequeue(Node** queue) {
    Node* temp = *queue;
    int data = temp->data;
    *queue = temp->next;
    free(temp);
    return data;
}

void wipe_visited_list(Graph* graph, int vertices) {
    for (int i = 0; i < vertices; i++) {
        graph->visited[i] = 0;
    }
}

// Graph Traversal Functions
void DFS(Graph* graph, int vertex) {
    graph->visited[vertex] = 1;
    printf("%d -> ", vertex);

    Node* temp = graph->adjacency_lists[vertex];
    while (temp) {
        int connected_vertex = temp->data;
        if (!graph->visited[connected_vertex]) {
            DFS(graph, connected_vertex);
        }
        temp = temp->next;
    }
}

void BFS(Graph* graph, int start) {
    Node* queue = NULL;
    
    graph->visited[start] = 1;
    enqueue(&queue, start);

    while (!is_empty(queue)) {
        int current = dequeue(&queue);
        printf("%d ", current);

        Node* temp = graph->adjacency_lists[current];
        while (temp) {
            int adj_vertex = temp->data;
            if (!graph->visited[adj_vertex]) {
                graph->visited[adj_vertex] = 1;
                enqueue(&queue, adj_vertex);
            }
            temp = temp->next;
        }
    }
}

// Main Function
int main() {
    int vertices, edges, starting_vertex;

    printf("How many vertices in the graph? ");
    scanf("%d", &vertices);
    
    printf("How many edges in the graph? ");
    scanf("%d", &edges);

    Graph* graph = create_graph(vertices);
    insert_edges(vertices, edges, graph);

    printf("Starting vertex for DFS? ");
    scanf("%d", &starting_vertex);
    printf("DFS traversal: ");
    DFS(graph, starting_vertex);
    printf("\n");

    wipe_visited_list(graph, vertices);
    
    printf("Starting vertex for BFS? ");
    scanf("%d", &starting_vertex);
    printf("BFS traversal: ");
    BFS(graph, starting_vertex);
    printf("\n");

    return 0;
}
