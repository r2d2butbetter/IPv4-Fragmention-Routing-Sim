#ifndef NETWORK_H
#define NETWORK_H

#include <stdbool.h>

#define MAX_NODES 20

typedef struct network_topology
{
    int node_count;
    int graph[MAX_NODES][MAX_NODES];
}network_topology;

// Initialize a new empty network topology
void init_network_topology(network_topology* network, int num_nodes);

// Add a connection (edge) between two nodes
void add_connection(network_topology* network, int from, int to, int weight);

// Create a predefined test topology
void create_test_topology(network_topology* network);

// Create a custom topology based on user input
void create_custom_topology(network_topology* network);

// Modify the network topology (for dynamic routing simulation)
void modify_network_topology(network_topology* network);

// Check if node is valid in the given network
bool is_valid_node(network_topology* network, int node);

#endif