#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INT_MIN -1

//Problem variables
int n, m, q;

//Tarjan algorithm variables
int t = 0;
int adj[100000][100000];
int low[100000];
int dfs[100000];

//Minimum value function
int min(int a, int b)
{
    return a < b ? a : b;
}

//==== QUEUE IMPLEMENTATION FROM GEEKSFORGEEKS
//https://www.geeksforgeeks.org/queue-set-1introduction-and-array-implementation/

// A structure to represent a queue
struct Queue
{
    int front, rear;
    unsigned size, capacity;
    int *array;
};

// function to create a queue
// of given capacity.
// It initializes size of queue as 0
struct Queue *createQueue(int capacity)
{
    struct Queue *queue = (struct Queue *)malloc(
        sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;

    // This is important, see the enqueue
    queue->rear = capacity - 1;
    queue->array = (int *)malloc(
        queue->capacity * sizeof(int));
    return queue;
}

// Queue is full when size becomes
// equal to the capacity
int isFull(struct Queue *queue)
{
    return (queue->size == queue->capacity);
}

// Queue is empty when size is 0
int isEmpty(struct Queue *queue)
{
    return (queue->size == 0);
}

// Function to get front of queue
int front(struct Queue *queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->front];
}

// Function to get rear of queue
int rear(struct Queue *queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->rear];
}

// Function to add an item to the queue.
// It changes rear and size
void push(struct Queue *queue, int item)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
}

// Function to remove an item from queue.
// It changes front and size
int dequeue(struct Queue *queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

void p3(int v)
{
    int n_circuits, n_POIS, longest_lane, total_lane_length = 0;

    int w;

    low[v] = dfs[v] = t;
    t = t + 1;

    struct Queue *S = createQueue(100000);

    push(S, v);
    for (int i = 1; i <= adj[v][0]; i++)
    {
        w = adj[v][0];
        if (dfs[w] == 0)
        {
            p3(w);
            low[v] = min(low[v], low[w]);
        }
        else if (w == front(S))
        {
            low[v] = min(low[v], dfs[w]);
        }
    }

    if (low[v] = dfs)
    {
        struct Queue *C = createQueue(100000);
        do
        {
            w = pop(S);
            push(C, w);
        } while (w != v);

        struct Queue *Scc = createQueue(100000);

        push(Scc, C);
    }

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

        memset(adj, 0, sizeof(int) * m * m);

        for (int j = 0; j < m; j++)
        {
            scanf("%d %d %d", &A, &B, &D);
            adj[A - 1][B - 1] = D;
        }
        p3(adj);
    }

    return 0;
}