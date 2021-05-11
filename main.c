#include <stdlib.h>
#include <stdio.h>

int n, m, q;

void p3(int adj[m][m])
{
    int n_circuits, n_POIS, longest_lane, total_lane_length;
    if (q == 1)
    {
        printf("%d\n", n_circuits);
    }
    else if (q == 2)
    {
        printf("%d %d\n", n_circuits, n_POIS);
    }
    else if (q == 3)
    {
        printf("%d %d %d\n", n_circuits, n_POIS, longest_lane);
    }
    else if (q == 4)
    {
        printf("%d %d %d %d\n", n_circuits, n_POIS, longest_lane, total_lane_length);
    }
}

int main()
{
    int tests;
    scanf("%d", &tests);

    for (int i = 0; i < tests; i++)
    {
        scanf("%d %d %d", &n, &m, &q);

        int A, B, D;
        int adj[m][m];
        for (int j = 0; j < m; j++)
        {
            scanf("%d %d %d", &A, &B, &D);
            adj[A - 1][B - 1] = D;
            adj[B - 1][A - 1] = D;
        }
        p3(adj);
    }

    return 0;
}