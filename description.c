#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int n, m, q;
int adj[1001][1001][2];
int circuits[1001];
int num_elements[1001];

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
                        if(circuits[i] == 0 && circuits[k] == 0)
                        {
                            circuits[i] = group_num;
                            circuits[k] = group_num;
                            group_num++;
                        }
                        else if(circuits[i] != 0 && circuits[k] != 0 && circuits[i] != circuits[k])
                        {
                            if(circuits[i] < circuits[k])
                            {
                                circuits[k] = circuits[i];
                            }
                            else
                            {
                                circuits[i] = circuits[k];
                            }
                        }
                        else
                        {
                            if(circuits[i] == 0)
                            {
                                circuits[i] = circuits[k];
                            }
                            else
                            {
                                circuits[k] = circuits[i];
                            }
                        }
                    }
                }
            }
        }
        print_circuits();
    }
}

int num_circuits()
{
    int max = 0;

    for(int i = 1; i <= n; i++)
    {
        if(circuits[i] > max)
        {
            max = circuits[i];
        }
    }

    return max;
}

int get_num_elements()
{
    int max = 0;

    for(int i = 1; i <= n; i++)
    {
        if(circuits[i] > 0)
        {
            num_elements[circuits[i]]++;
        }
        
        if(num_elements[circuits[i]] > max)
        {
            max = num_elements[circuits[i]];
        }
    }

    return max;
}

int main()
{
    int n_tests;
    scanf("%d", &n_tests);

    while (n_tests > 0)
    {
        scanf("%d %d %d", &n, &m, &q);
        memset(adj, 0, (n + 1) * (n + 1) * 2 * sizeof(*adj));
        memset(circuits, 0, (n + 1) * sizeof(*circuits));
        
        int A, B, W;
        for (int i = 0; i < m; i++)
        {
            scanf("%d %d %d", &A, &B, &W);

            adj[A][B][0] = W;
            adj[A][B][1] = 1;
        }
    
        get_circuits();
        printf("%d", num_circuits());

        if(q > 1)
        {
            memset(num_elements, 0, (n + 1) * sizeof(*num_elements));
            printf(" %d", get_num_elements());
        }
    
        printf("\n");
        n_tests--;
    }
}