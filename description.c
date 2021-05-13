#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int n, m, q;
int adj[1001][1001][2];
int circuits[1001];

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
    for(int i = 1; i <= n; i++)
    {
        printf("%d ", circuits[i]);
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
                if(adj[i][k][1] == 1 && adj[k][j][1] == 1)
                {
                    adj[i][j][1] = 1;
                    if(j == i)
                    {
                        if(adj[k][k][1] == 1)
                        {
                            circuits[i] = circuits[k];
                        }
                        else
                        {
                            circuits[i] = group_num;
                            group_num++;
                        }
                    }
                }
            }
        }
    }
}

int main()
{
    int n_tests;
    scanf("%d", &n_tests);

    while (n_tests > 0)
    {
        scanf("%d %d %d", &n, &m, &q);
        memset(adj, 0, (n + 1) * (n + 1) * 2 * sizeof(int));
        memset(circuits, 0, (n + 1) * sizeof(int));

        int A, B, W;
        for (int i = 0; i < m; i++)
        {
            scanf("%d %d %d", &A, &B, &W);

            adj[A][B][0] = W;
            adj[A][B][1] = 1;
        }

    
        print_adj();
        get_circuits();
        printf("-------------------------------\n");
        print_adj();
        printf("-------------------------------\n");
        print_circuits();
        n_tests--;
    }
}