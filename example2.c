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

Node* create_node(int value) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        printf("Eroare alocare memorie pentru nod!\n");
        exit(1);
    }
    new_node->data = value;
    new_node->next = NULL;
    return new_node;
}

Graph* create_graph(int vertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (!graph) {
        printf("Eroare alocare memorie pentru graf!\n");
        exit(1);
    }
    graph->vertices = vertices;
    graph->adjacency_lists = (Node**)malloc(vertices * sizeof(Node*));
    graph->visited = (int*)malloc(vertices * sizeof(int));
    if (!graph->adjacency_lists || !graph->visited) {
        printf("Eroare alocare memorie pentru liste/visited!\n");
        exit(1);
    }
    for (int i = 0; i < vertices; i++) {
        graph->adjacency_lists[i] = NULL;
        graph->visited[i] = 0;
    }
    return graph;
}

void add_edge(Graph* graph, int src, int dest) {
    if (src < 0 || src >= graph->vertices || dest < 0 || dest >= graph->vertices) {
        printf("Muchie invalida: %d -> %d (max %d)\n", src, dest, graph->vertices - 1);
        return;
    }
    Node* new_node = create_node(dest);
    new_node->next = graph->adjacency_lists[src];
    graph->adjacency_lists[src] = new_node;

    new_node = create_node(src);
    new_node->next = graph->adjacency_lists[dest];
    graph->adjacency_lists[dest] = new_node;
}

void insert_edges(int vertices, int edges, Graph* graph) {
    printf("Adauga %d muchii (de la 0 la %d):\n", edges, vertices - 1);
    for (int i = 0; i < edges; i++) {
        int src, dest;
        printf("Muchia %d : ", i + 1);
        if (scanf("%d %d", &src, &dest) != 2) {
            printf("Eroare citire muchie!\n");
            exit(1);
        }
        add_edge(graph, src, dest);
    }
}

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
    if (is_empty(*queue)) {
        printf("Eroare: coada este goala!\n");
        exit(1);
    }
    Node* temp = *queue;
    int data = temp->data;
    *queue = temp->next;
    free(temp);
    return data;
}

void wipe_visited_list(Graph* graph) {
    for (int i = 0; i < graph->vertices; i++) {
        graph->visited[i] = 0;
    }
}

void DFS(Graph* graph, int vertex) {
    if (vertex < 0 || vertex >= graph->vertices || graph->visited[vertex]) {
        return;
    }
    graph->visited[vertex] = 1;
    printf("%d ", vertex);

    Node* temp = graph->adjacency_lists[vertex];
    while (temp) {
        int connected_vertex = temp->data;
        DFS(graph, connected_vertex);
        temp = temp->next;
    }
}

void BFS(Graph* graph, int start) {
    if (start < 0 || start >= graph->vertices) {
        printf("Varf de start invalid: %d (max %d)\n", start, graph->vertices - 1);
        return;
    }
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

void free_graph(Graph* graph) {
    for (int i = 0; i < graph->vertices; i++) {
        Node* temp = graph->adjacency_lists[i];
        while (temp) {
            Node* next = temp->next;
            free(temp);
            temp = next;
        }
    }
    free(graph->adjacency_lists);
    free(graph->visited);
    free(graph);
}

int main() {
    int vertices, edges, start_dfs, start_bfs;

    printf("Cate varfuri are graful? ");
    if (scanf("%d", &vertices) != 1 || vertices <= 0) {
        printf("Numar invalid de varfuri!\n");
        return 1;
    }

    printf("Cate muchii are graful? ");
    if (scanf("%d", &edges) != 1 || edges < 0) {
        printf("Numar invalid de muchii!\n");
        return 1;
    }

    Graph* graph = create_graph(vertices);
    insert_edges(vertices, edges, graph);

    printf("Varf de start pentru DFS? ");
    if (scanf("%d", &start_dfs) != 1) {
        printf("Eroare citire varf DFS!\n");
        free_graph(graph);
        return 1;
    }
    printf("DFS: ");
    DFS(graph, start_dfs);
    printf("\n");

    wipe_visited_list(graph);

    printf("Varf de start pentru BFS? ");
    if (scanf("%d", &start_bfs) != 1) {
        printf("Eroare citire varf BFS!\n");
        free_graph(graph);
        return 1;
    }
    printf("BFS: ");
    BFS(graph, start_bfs);
    printf("\n");

    free_graph(graph);
    return 0;
}
