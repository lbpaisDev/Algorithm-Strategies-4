#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct edges
{
    int edge[2];
    int weight;
    struct edges *next;
} Edges;

int n, m, q;
int adj[1001][1001][2];
int circuits_verts[1001];
int n_elements[501];
int verts[1001];
int set[1001];
int rank[1001];
Edges *edges_list;

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

void print_adj()
{
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            printf("[%d %d] ", adj[i][j][0], adj[i][j][1]);
        }
        printf("\n");
    }
}

void print_circuits()
{
    printf("[");
    for (int i = 1; i <= n; i++)
    {
        printf("%d ", circuits_verts[i]);
    }
    printf("]\n");
}

void get_circuits()
{
    int group_num = 1;
    for (int k = 1; k <= n; k++)
    {
        for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                if (adj[i][k][1] == 1 && adj[k][j][1] == 1)
                {
                    adj[i][j][1] = 1;
                    if (j == i)
                    {
                        if (circuits_verts[i] == 0 && circuits_verts[k] == 0)
                        {
                            circuits_verts[i] = group_num;
                            circuits_verts[k] = group_num;
                            group_num++;
                        }
                        else if (circuits_verts[i] != 0 && circuits_verts[k] != 0 && circuits_verts[i] != circuits_verts[k])
                        {
                            if (circuits_verts[i] < circuits_verts[k])
                            {
                                circuits_verts[k] = circuits_verts[i];
                            }
                            else
                            {
                                circuits_verts[i] = circuits_verts[k];
                            }
                        }
                        else
                        {
                            if (circuits_verts[i] == 0)
                            {
                                circuits_verts[i] = circuits_verts[k];
                            }
                            else
                            {
                                circuits_verts[k] = circuits_verts[i];
                            }
                        }
                    }
                }
            }
        }
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
            if (adj[verts[i]][verts[j]][0] != 0)
            {
                int edge[2] = {verts[i], verts[j]};
                insert_edge(edge, adj[verts[i]][verts[j]][0]);
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
        sum += adj[lane[i][0]][lane[i][1]][0];
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
        memset(adj, 0, (n + 1) * (n + 1) * 2 * sizeof(*adj));
        memset(circuits_verts, 0, (n + 1) * sizeof(*circuits_verts));

        int A, B, W;
        for (int i = 0; i < m; i++)
        {
            scanf("%d %d %d", &A, &B, &W);

            adj[A][B][0] = W;
            adj[A][B][1] = 1;
        }

        get_circuits();
        int num_circuits = get_num_circuits();
        printf("%d", num_circuits);

        if (q > 1)
        {
            memset(n_elements, 0, (n + 1) * sizeof(*n_elements));
            printf(" %d", get_num_elements());
        }

        int lane_comps[num_circuits];
        if (q > 2)
        {
            for (int i = 1; i <= num_circuits; i++)
            {
                int num_elements = n_elements[i];
                get_verts(i);
                edges_list = NULL;
                int num_edges = get_sorted_edges(num_elements);
                lane_comps[i - 1] = get_lane_comps(num_elements, num_edges);
            }

            printf(" %d", longest_lane(lane_comps, num_circuits));
        }

        if (q > 3)
        {
            printf(" %d", sum_lanes(lane_comps, num_circuits));
        }

        printf("\n");
        n_tests--;
    }

    return 0;
}