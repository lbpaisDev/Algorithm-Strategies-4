#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int n, m, q;

int adj[1001][1001][2];

//===== Stack de Pontos Fortemente Conexos
int stack[1001];
int top_stack;
int is_stacked[1001];

void push_stack(int v)
{
    top_stack++;
    stack[top_stack] = v;
    is_stacked[v] = 1;
}

int pop_stack()
{
    int w = stack[top_stack];
    stack[top_stack] = -1;
    is_stacked[w] = 0;
    top_stack--;
    return w;
}
//=====

//===== Matriz de circuitos
int circuits[501][1001];
int top_circuit;
int n_circuit;

void push_circuit(int w)
{
    top_circuit++;
    circuits[n_circuit][top_circuit] = w;
}
//=====

// Get min of two numbers
int min(int a, int b)
{
    return (a < b) ? a : b;
}

//=====
int low[1001];
int dfs[1001];
int t;

void tarjan(int v)
{
    low[v] = dfs[v] = t;
    t++;
    push_stack(v);
    for (int w = 1; w <= n; w++)
    {
        if (adj[v][w][1] == 0)
        {
            continue;
        }

        if (dfs[w] == -1)
        {
            tarjan(w);
            low[v] = min(low[v], low[w]);
        }
        else if (is_stacked[w])
        {
            low[v] = min(low[v], dfs[w]);
        }
    }

    if (low[v] == dfs[v])
    {
        n_circuit++;
        top_circuit = 0;
        int w = 0;
        while (w != v)
        {
            w = pop_stack();
            push_circuit(w);
        }

        if (top_circuit == 1) // so ha circuitos com 2 ou mais vertices
        {
            circuits[n_circuit][0] = 0;
            n_circuit--;
        }
        else
        {
            circuits[n_circuit][0] = top_circuit;
        }
    }
}

int get_circuits()
{
    memset(dfs, -1, sizeof(dfs));
    memset(low, -1, sizeof(low));
    memset(is_stacked, 0, sizeof(is_stacked));
    top_stack = 0;
    top_circuit = 0;
    n_circuit = 0;

    //Call tarjan for every node
    for (int i = 1; i < n + 1; i++)
    {
        //Only if its not visited already
        if (dfs[i] == -1)
        {
            tarjan(i);
        }
    }

    circuits[0][0] = n_circuit;
    return circuits[0][0];
}

int get_max_circuit()
{
    int max = 0;
    for (int i = 1; i <= circuits[i][0]; i++)
    {
        if (circuits[i][0] > max)
        {
            max = circuits[i][0];
        }
    }
    return max;
}
//=====

//=====
typedef struct edges
{
    int edge[2];
    int weight;
    struct edges *next;
} Edges;
Edges *edges_list;

void free_edges_list()
{
    Edges *edge;
    while (edges_list != NULL)
    {
        edge = edges_list;
        edges_list = edges_list->next;
        free(edge);
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

void get_sorted_edges(int verts[n + 1], int num_verts)
{

    for (int i = 1; i <= num_verts; i++)
    {
        for (int j = 1; j <= num_verts; j++)
        {
            if (adj[verts[i]][verts[j]][0] != 0)
            {
                int edge[2] = {verts[i], verts[j]};

                if(adj[verts[j]][verts[i]][0] != 0 && adj[verts[j]][verts[i]][0] < adj[verts[i]][verts[j]][0])
                {
                    continue;
                }
                
                insert_edge(edge, adj[verts[i]][verts[j]][0]);
            }
        }
    }
}
//=====

int set[1001];
int rank[1001];
int num_verts[1001];

void make_set(int num_elements, int verts[n + 1])
{
    for (int i = 1; i <= num_elements; i++)
    {
        set[verts[i]] = verts[i];
        num_verts[verts[i]] = 1;
        rank[verts[i]] = 0;
    }
}

int find_set(int a)
{
    if (set[a] != a)
    {
        set[a] = find_set(set[a]);
    }
    return set[a];
}

int link(int a, int b)
{
    int result;

    if (rank[a] > rank[b])
    {
        set[b] = a;
        num_verts[a] += num_verts[b];
        result = num_verts[a];
    }
    else
    {
        set[a] = b;
        num_verts[b] += num_verts[a];
        result = num_verts[b];
    }

    if (rank[a] == rank[b])
    {
        rank[b]++;
    }

    return result;
}

int union_verts(int a, int b)
{
    return link(find_set(a), find_set(b));
}

int get_lane_comps(int num_vertices)
{
    int edge = 0, lane_sum = 0;

    Edges *edge_node = edges_list;
    while (edge_node != NULL)
    {
        if (find_set(edge_node->edge[0]) != find_set(edge_node->edge[1]))
        {
            lane_sum += edge_node->weight;
            edge++;

            if(union_verts(edge_node->edge[0], edge_node->edge[1]) == num_vertices)
            {
                return lane_sum;
            }
        }

        edge_node = edge_node->next;
    }
    
    return lane_sum;
}

int sum_lanes(int *lane_comps)
{
    int sum = 0;
    for (int i = 0; i < circuits[0][0]; i++)
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
        memset(adj, 0, sizeof(adj));

        int A, B, W;
        for (int i = 0; i < m; i++)
        {
            scanf("%d %d %d", &A, &B, &W);
            adj[A][B][0] = W;
            adj[A][B][1] = 1;
        }

        printf("%d", get_circuits());

        if (q > 1)
        {
            printf(" %d", get_max_circuit());
        }

        int lane_comps[circuits[0][0]];
        if (q > 2)
        {
            int max = 0;

            for (int i = 1; i <= circuits[0][0]; i++)
            {
                edges_list = NULL;
                get_sorted_edges(circuits[i], circuits[i][0]);
                
                make_set(circuits[i][0], circuits[i]);
                lane_comps[i - 1] = get_lane_comps(circuits[i][0]);
                free_edges_list();

                if(lane_comps[i - 1] > max)
                {
                    max = lane_comps[i - 1];
                }
            }

            printf(" %d", max);
        }

        if (q > 3)
        {
            printf(" %d", sum_lanes(lane_comps));
        }

        printf("\n");
        n_tests--;
    }

    return 0;
}