#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PLUGINS 100
#define MAX_NAME_LEN 50

typedef struct Node
{
    int dest;
    struct Node *next;
} Node;

typedef struct Graph
{
    int num_plugins;
    char plugin_names[MAX_PLUGINS][MAX_NAME_LEN];
    Node *adj_list[MAX_PLUGINS];
    int in_degree[MAX_PLUGINS];
} Graph;

Node *createNode(int dest)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}

Graph *createGraph(int num_plugins)
{
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->num_plugins = num_plugins;

    for (int i = 0; i < num_plugins; i++)
    {
        graph->adj_list[i] = NULL;
        graph->in_degree[i] = 0;
    }
    return graph;
}

void setPluginName(Graph *graph, int id, const char *name)
{
    if (id >= 0 && id < graph->num_plugins)
    {
        strncpy(graph->plugin_names[id], name, MAX_NAME_LEN - 1);
        graph->plugin_names[id][MAX_NAME_LEN - 1] = '\0';
    }
}

void addDependency(Graph *graph, int src, int dest)
{
    Node *newNode = createNode(dest);
    newNode->next = graph->adj_list[src];
    graph->adj_list[src] = newNode;
    graph->in_degree[dest]++;
}

void calculateLoadOrder(Graph *graph)
{
    int queue[MAX_PLUGINS];
    int front = 0, rear = 0;
    int sorted_order[MAX_PLUGINS];
    int processed_count = 0;
    for (int i = 0; i < graph->num_plugins; i++)
    {
        if (graph->in_degree[i] == 0)
        {
            queue[rear++] = i;
        }
    }
    while (front < rear)
    {
        int current_plugin = queue[front++];
        sorted_order[processed_count++] = current_plugin;
        Node *temp = graph->adj_list[current_plugin];
        while (temp != NULL)
        {
            int neighbor = temp->dest;
            graph->in_degree[neighbor]--;
            if (graph->in_degree[neighbor] == 0)
            {
                queue[rear++] = neighbor;
            }
            temp = temp->next;
        }
    }
    if (processed_count != graph->num_plugins)
    {
        printf("\nERROR! Circular Dependency Detected!\n");
        printf("The graph is not a valid DAG. A topological sort is impossible.\n");
        printf("The following plugins are caught in a circular loading loop:\n");
        for (int i = 0; i < graph->num_plugins; i++)
        {
            if (graph->in_degree[i] != 0)
            {
                printf(" - %s\n", graph->plugin_names[i]);
            }
        }
    }
    else
    {
        printf("\nSUCCESS! Valid Load Order Generated:\n");
        printf("--------------------------------------\n");
        for (int i = 0; i < processed_count; i++)
        {
            printf("%02d. %s\n", i + 1, graph->plugin_names[sorted_order[i]]);
        }
        printf("--------------------------------------\n");
    }
}

int main()
{
    int scenario;
    printf("Pick Scenario: ");
    scanf("%d", &scenario);
    if (scenario == 1) {
        printf("SCENARIO 1: Resolving a Valid Mod List\n");
        int num_mods = 5;
        Graph *g1 = createGraph(num_mods);

        setPluginName(g1, 0, "Skyrim.esm");
        setPluginName(g1, 1, "Update.esm");
        setPluginName(g1, 2, "WeatherOverhaul.esp");
        setPluginName(g1, 3, "WaterTextures.esp");
        setPluginName(g1, 4, "WeatherPatch.esp");

        addDependency(g1, 0, 1);
        addDependency(g1, 1, 2);
        addDependency(g1, 1, 3);
        addDependency(g1, 2, 4);
        addDependency(g1, 3, 4);

        calculateLoadOrder(g1);
    }

    else if (scenario == 2) {
        printf("\nSCENARIO 2: Invalid Mods List (Infinite Loop)\n");
        int num_mods = 3;
        Graph *g2 = createGraph(num_mods);

        setPluginName(g2, 0, "CombatOverhaul.esp");
        setPluginName(g2, 1, "AnimationAddon.esp");
        setPluginName(g2, 2, "WeaponsPack.esp");

        addDependency(g2, 0, 1);
        addDependency(g2, 1, 2);
        addDependency(g2, 2, 0);

        calculateLoadOrder(g2);
    }

    else if (scenario == 3) {
        printf("\nSCENARIO 3: Managing the 'Rule of One' via a Resolution Patch\n");
        int num_mods = 4;
        Graph *g3 = createGraph(num_mods);

        setPluginName(g3, 0, "Skyrim.esm");
        setPluginName(g3, 1, "WeaponDamageOverhaul.esp");
        setPluginName(g3, 2, "WeaponMeshReplacer.esp");
        setPluginName(g3, 3, "Weapon_ConsistencyPatch.esp");

        addDependency(g3, 0, 1);
        addDependency(g3, 0, 2);
        addDependency(g3, 1, 3);
        addDependency(g3, 2, 3);

        calculateLoadOrder(g3);
    }
    return 0;
}