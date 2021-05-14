#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEBUG 0

int n, m, q;

int adj[1001][1001];
int dist[1001][1001];

int t;
int n_circuits;
int n_pois;

int n_elements[501];
int verts[1001];
int set[1001];
int rank[1001];
int circuits_verts[1001];

//===== DEBUG FUNCTIONS
void print_aux_arrays(int arr[], char *array)
{
    printf("%s ->", array);
    for (int i = 1; i < n + 1; i++)
    {
        printf(" %d", arr[i]);
    }
    printf("\n");
}

void print_adj()
{
    printf("-> ADJACENCY MATRIX\n");
    for (int i = 1; i < n + 1; i++)
    {
        printf("[ ");
        for (int j = 1; j < n + 1; j++)
        {
            printf("%d ", adj[i][j]);
        }
        printf(" ]\n");
    }
}

void print_dist()
{
    printf("-> DISTANCE MATRIX\n");
    for (int i = 1; i < n + 1; i++)
    {
        printf("[ ");
        for (int j = 1; j < n + 1; j++)
        {
            printf("%d ", dist[i][j]);
        }
        printf(" ]\n");
    }
}

//===== END OF DEBUG FUNCTIONS

//===== STACK IMPLEMENTATION
//Stack structure
struct Stack
{
    int top;
    int capacity;
    int *array;
};

//Allocate memory for stack
struct Stack *create_stack(unsigned capacity)
{
    struct Stack *stack = (struct Stack *)malloc(sizeof(struct Stack));

    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (int *)malloc(stack->capacity * sizeof(int));

    return stack;
}

//Check if the stack is full
int is_full(struct Stack *stack)
{
    return stack->top == stack->capacity - 1;
}

//Check if the stack is empty
int is_empty(struct Stack *stack)
{
    return stack->top == -1;
}

//Push into the stack
void push(struct Stack *stack, int data)
{
    //If there's no space left return
    if (is_full(stack))
        return;

    stack->array[++stack->top] = data;

    if (DEBUG)
        printf("%d pushed to stack\n", data);
}

//Pop an item from the stack
int pop(struct Stack *stack)
{
    //If its empty do nothing
    if (is_empty(stack))
        return -1;

    int popped = stack->array[stack->top--];
    if (DEBUG)
        printf("%d popped from stack\n", popped);

    return popped;
}

//See data on top
int peek(struct Stack *stack)
{
    if (is_empty(stack))
    {
        return -1;
    }
    return stack->array[stack->top];
}
//===== END OF STACK IMPLEMENTATION

//===== LINKED LISTS OF LINKED LIST IMPLEMENTATION

typedef struct poi_list
{
    int vertex;
    struct poi_list *next;

} POIs;

typedef struct circuits
{
    POIs *poi;

    struct circuits *next;
} Circuit;

Circuit *head_circuit = NULL;

typedef struct edges
{
    int edge[2];
    int weight;
    struct edges *next;

} Edges;

Edges *edges_list;

POIs *create_poi(int vertex, POIs *next)
{
    POIs *new_node = (POIs *)malloc(sizeof(POIs));

    new_node->vertex = vertex;
    new_node->next = next;

    return new_node;
}

POIs *insert_poi(POIs *head, int vertex)
{
    if (head == NULL)
        return NULL;

    POIs *cursor = head;
    while (cursor->next != NULL)
        cursor = cursor->next;

    POIs *new_node = create_poi(vertex, NULL);
    cursor->next = new_node;

    return head;
}

void delete_poi(POIs *header)
{
    POIs *cursor, *tmp;

    if (header != NULL)
    {
        cursor = header->next;
        header->next = NULL;
        while (cursor != NULL)
        {
            tmp = cursor->next;
            free(cursor);
            cursor = tmp;
        }
    }
}

void print_pois(POIs *header)
{
    if (header == NULL)
    {
        return;
    }
    POIs *ptr = header;
    while (ptr != NULL)
    {
        if (ptr->vertex != -1)
            printf(" %d ", ptr->vertex);
        ptr = ptr->next;
    }
    printf("\n");
}

Circuit *create_circuit(POIs *header, Circuit *next)
{
    Circuit *new_node = (Circuit *)malloc(sizeof(Circuit));

    new_node->poi = header;
    new_node->next = next;

    return new_node;
}

Circuit *insert_circuit(Circuit *head, POIs *header)
{
    if (head == NULL)
        return NULL;

    Circuit *cursor = head;
    while (cursor->next != NULL)
        cursor = cursor->next;

    Circuit *new_node = create_circuit(header, NULL);
    cursor->next = new_node;

    return head;
}

void delete_circuit(Circuit *header)
{
    Circuit *cursor, *tmp;

    if (header != NULL)
    {
        cursor = header->next;
        header->next = NULL;
        while (cursor != NULL)
        {
            tmp = cursor->next;
            delete_poi(cursor->poi);
            free(cursor);
            cursor = tmp;
        }
    }
}

void print_circuit(Circuit *header)
{
    if (header == NULL)
    {
        return;
    }
    Circuit *ptr = header;
    while (ptr != NULL)
    {
        print_pois(ptr->poi);
        ptr = ptr->next;
    }
}

void insert_edge(int edge[2], int weight)
{
    Edges *new_edge = (Edges *)malloc(sizeof(Edges));
    new_edge->edge[0] = edge[0];
    new_edge->edge[1] = edge[1];
    new_edge->weight = weight;
    new_edge->next = NULL;

    Edges *actual = edges_list, *prev = NULL;
    while (actual != NULL && actual->weight <= weight)
    {
        prev = actual;
        actual = actual->next;
    }

    if (actual == NULL && prev == NULL)
    {
        edges_list = new_edge;
        return;
    }

    if (actual != NULL)
    {
        new_edge->next = actual;
    }

    if (prev != NULL)
    {
        prev->next = new_edge;
    }
    else
    {
        edges_list = new_edge;
    }
}

//===== END OF LINKED LISTS OF LINKED LIST IMPLEMENTATION

//===== AUXILIARY FUNCTIONS
//Return min of two numbers
int min(int a, int b)
{
    return (a < b) ? a : b;
}
//===== END OF AUXILIARY FUNCTIONS

void tarjan(int u, int visited[], int low[], int is_stacked[], struct Stack *stk)
{
    visited[u] = low[u] = ++t;

    push(stk, u);
    is_stacked[u] = 1;

    for (int i = 1; i < n + 1; i++)
    {
        int v = i;
        if (adj[u][v])
        {
            if (visited[v] == -1)
            {
                tarjan(v, visited, low, is_stacked, stk);
                low[u] = min(low[u], low[v]);
            }
            else if (is_stacked[v] == 1)
            {
                low[u] = min(low[u], visited[v]);
            }
        }
    }

    int w = 0;
    if (low[u] == visited[u])
    {
        //Create header for list poi
        POIs *head_poi = create_poi(-1, NULL);

        int i;
        for (i = 0; peek(stk) != u; i++)
        {
            w = pop(stk);
            is_stacked[w] = 0;

            head_poi = insert_poi(head_poi, w);
        }

        i++;
        w = pop(stk);
        head_poi = insert_poi(head_poi, w);

        //Only circuits with 2 or more points of interest count
        //answer 1
        if (i >= 2)
        {
            n_circuits++;
            if (head_poi != NULL)
                head_circuit = insert_circuit(head_circuit, head_poi);
        }
        else
        {
            //Otherwise delete list
            delete_poi(head_poi);
        }

        //If i = 1 its not considered a circuit so don't update
        //Else update  (answer 2)
        if (i > n_pois && i != 1)
            n_pois = i;

        is_stacked[w] = 0;
    }
}

void dfs()
{
    //Set arrays
    int visited[n + 1];
    int low[n + 1];
    int is_stacked[n + 1];
    struct Stack *stk = create_stack(n + 1);

    //Initialize arrays
    memset(visited, -1, (n + 1) * sizeof(int));
    memset(low, -1, (n + 1) * sizeof(int));
    memset(is_stacked, 0, (n + 1) * sizeof(int));

    if (DEBUG)
    {
        print_aux_arrays(visited, "visited");
        print_aux_arrays(low, "low");
        print_aux_arrays(is_stacked, "is_stacked");
        printf("\n");
    }

    //Call tarjan for every node
    for (int i = 1; i < n + 1; i++)
    {
        //Only if its not visited already
        if (visited[i] == -1)
        {
            tarjan(i, visited, low, is_stacked, stk);
        }
    }
}

void get_circuits_verts()
{
    Circuit *ptr_circuit = head_circuit;
    for (int i = 0; ptr_circuit != NULL; i++)
    {
        POIs *ptr_poi = ptr_circuit->poi;
        for (int j = 0; ptr_poi != NULL; j++)
        {
            circuits_verts[ptr_poi->vertex] = i;
            ptr_poi = ptr_poi->next;
        }
        ptr_circuit = ptr_circuit->next;
    }
}

int get_num_circuits()
{
    int max = 0;

    for (int i = 1; i <= n; i++)
    {
        if (circuits_verts[i] > max)
        {
            max = circuits_verts[i];
        }
    }

    return max;
}

int get_num_elements()
{
    int max = 0;

    for (int i = 1; i <= n; i++)
    {
        if (circuits_verts[i] > 0)
        {
            n_elements[circuits_verts[i]]++;
        }

        if (n_elements[circuits_verts[i]] > max)
        {
            max = n_elements[circuits_verts[i]];
        }
    }

    return max;
}

void get_verts(int num_circuit)
{
    int vert = 1;
    for (int i = 1; i <= n; i++)
    {
        if (circuits_verts[i] == num_circuit)
        {
            verts[vert] = i;
            vert++;
        }
    }
}

void make_set(int num_elements)
{
    for (int i = 1; i <= num_elements; i++)
    {
        set[verts[i]] = verts[i];
        rank[verts[i]] = 0;
    }
}

int get_sorted_edges(int num_elements)
{
    int num_edges = 0;

    for (int i = 1; i <= num_elements; i++)
    {
        for (int j = 1; j <= num_elements; j++)
        {
            if (dist[verts[i]][verts[j]] != 0)
            {
                int edge[2] = {verts[i], verts[j]};
                insert_edge(edge, dist[verts[i]][verts[j]]);
                num_edges++;
            }
        }
    }

    return num_edges;
}

int find_set(int a)
{
    if (set[a] != a)
    {
        set[a] = find_set(set[a]);
    }
    return set[a];
}

void link(int a, int b)
{
    if (rank[a] > rank[b])
    {
        set[b] = a;
    }
    else
    {
        set[a] = b;
    }

    if (rank[a] == rank[b])
    {
        rank[b]++;
    }
}

void union_verts(int a, int b)
{
    link(find_set(a), find_set(b));
}

int sum_lane(int lane[][2], int num_edges)
{
    int sum = 0;
    for (int i = 0; i < num_edges; i++)
    {
        sum += dist[lane[i][0]][lane[i][1]];
    }

    return sum;
}

int get_lane_comps(int num_elements, int num_edges)
{
    make_set(num_elements);

    int lane[num_edges][2], edge = 0;
    while (edges_list != NULL)
    {
        if (find_set(edges_list->edge[0]) != find_set(edges_list->edge[1]))
        {
            lane[edge][0] = edges_list->edge[0];
            lane[edge][1] = edges_list->edge[1];
            edge++;
            union_verts(edges_list->edge[0], edges_list->edge[1]);
        }

        Edges *prev = edges_list;
        edges_list = edges_list->next;
        free(prev);
    }

    return sum_lane(lane, edge);
}

int longest_lane(int *lane_comps, int num_circuits)
{
    int max = 0;
    for (int i = 0; i < num_circuits; i++)
    {
        if (lane_comps[i] > max)
        {
            max = lane_comps[i];
        }
    }
    return max;
}

int sum_lanes(int *lane_comps, int num_circuits)
{
    int sum = 0;
    for (int i = 0; i < num_circuits; i++)
    {
        sum += lane_comps[i];
    }

    return sum;
}

int main()
{
    int n_tests;
    scanf("%d", &n_tests);

    while (n_tests > 0)
    {
        scanf("%d %d %d", &n, &m, &q);

        //Set matrices values
        memset(adj, 0, sizeof(adj));
        memset(dist, 0, sizeof(dist));

        //Set up adjacency and distances matrices
        int A, B, D;
        for (int i = 0; i < m; i++)
        {
            scanf("%d %d %d", &A, &B, &D);

            adj[A][B] = 1;
            dist[A][B] = D;
        }

        //Reset global counters for each test
        t = 0;
        n_circuits = 0;
        n_pois = 0;
        head_circuit = create_circuit(NULL, NULL);

        if (DEBUG)
        {
            print_adj();
            printf("\n");
            print_dist();
            printf("\n");
        }

        //Driver function
        dfs();

        get_circuits_verts();
        printf("%d", n_circuits);

        if (q > 1)
        {
            memset(n_elements, 0, (n + 1) * sizeof(*n_elements));
            get_num_elements();

            printf(" %d", n_pois);
        }

        int lane_comps[n_circuits];
        if (q > 2)
        {
            for (int i = 1; i <= n_circuits; i++)
            {
                int num_elements = n_elements[i];
                get_verts(i);
                edges_list = NULL;
                int num_edges = get_sorted_edges(num_elements);
                lane_comps[i - 1] = get_lane_comps(num_elements, num_edges);
            }

            printf(" %d", longest_lane(lane_comps, n_circuits));
        }

        if (q > 3)
        {
            printf(" %d", sum_lanes(lane_comps, n_circuits));
        }

        printf("\n");

        delete_circuit(head_circuit);

        n_tests--;
    }

    return 0;
}