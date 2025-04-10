#include "include/network.h"

#include <stdio.h>
#include <stdlib.h>

void init_network_topology(network_topology* network, int num_nodes) {
  if (num_nodes > MAX_NODES) {
    printf("Number of nodes is more than max, resetting to max");

    num_nodes = MAX_NODES;
  }

  network->node_count = num_nodes;

  for (int i = 0; i < num_nodes; i++) {
    for (int j = 0; j < num_nodes; j++) {
      network->graph[i][j] = 0;
    }
  }
}

void add_connection(network_topology* network, int from, int to, int weight) {
  if ((from >= network->node_count || from < 0) ||
      (to >= network->node_count || to < 0)) {
    printf("Error: Invalid node specified");
    return;
  }

  network->graph[from][to] = weight;
}

void create_test_topology(network_topology* network) {
  init_network_topology(network, 6); //6nodes

  // Add connections (directed graph)
  add_connection(network, 0, 1, 7);  // Node 0 -> Node 1, weight 7
  add_connection(network, 0, 2, 12);  // Node 0 -> Node 2, weight 12
  add_connection(network, 1, 2, 2);  // Node 1 -> Node 2, weight 2
  add_connection(network, 1, 3, 9);  // Node 1 -> Node 3, weight 9
  add_connection(network, 2, 4, 10); // Node 2 -> Node 4, weight 10
  add_connection(network, 4, 3, 4);  // Node 4 -> Node 3, weight 4
  add_connection(network, 3, 5, 1);  // Node 3 -> Node 5, weight 1
  add_connection(network, 4, 5, 5);  // Node 4 -> Node 5, weight 5

  printf("Test topology created with 6 nodes.\n");
}

void create_custom_topology(network_topology* network) {
  int num_nodes, num_connections;

  printf("Enter the number of nodes: ");
  scanf("%d", &num_nodes);

  init_network_topology(network, num_nodes);

  printf("Enter the number of connectiosn: ");
  scanf("%d", &num_connections);

  printf("Enter connections as [FROM] [TO] [WEIGHT]");
  for (int i = 0; i < num_connections; i++) {
    int from, to, weight;

    printf("Connection %d", i + 1);
    scanf("%d %d %d", &from, &to, &weight);

    if (from < 0 || from >= num_nodes || to < 0 || to >= num_nodes) {
      printf("Invalid nodes. Both must be between 0 and %d. Try again.\n",
             num_nodes - 1);
      i--;  // Retry this connection
    } else if (weight <= 0) {
      printf("Weight must be positive. Try again.\n");
      i--;  // Retry this connection
    } else {
      add_connection(network, from, to, weight);
    }
  }

  printf("Custom graph created!!");
}

void modify_network_topology(network_topology* network) {
  printf("\n=== Modifying Network Topology ===\n");
  printf("1. Add a new connection\n");
  printf("2. Remove an existing connection\n");
  printf("3. Change the weight of a connection\n");
  printf("Enter your choice (1-3): ");

  int choice;
  scanf("%d", &choice);

  int from, to, weight;

  switch (choice) {
    case 1:  // Add connection
      printf("Enter new connection (from to weight): ");
      scanf("%d %d %d", &from, &to, &weight);
      if (from >= 0 && from < network->node_count && to >= 0 &&
          to < network->node_count && weight > 0) {
        add_connection(network, from, to, weight);
        printf("Connection added: %d -> %d (weight: %d)\n", from, to, weight);
      } else {
        printf("Invalid input. No changes made.\n");
      }
      break;

    case 2:  // Remove connection
      printf("Enter connection to remove (from to): ");
      scanf("%d %d", &from, &to);
      if (from >= 0 && from < network->node_count && to >= 0 &&
          to < network->node_count) {
        network->graph[from][to] = 0;
        printf("Connection removed: %d -> %d\n", from, to);
      } else {
        printf("Invalid input. No changes made.\n");
      }
      break;

    case 3:  // Change weight
      printf("Enter connection to modify (from to new_weight): ");
      scanf("%d %d %d", &from, &to, &weight);
      if (from >= 0 && from < network->node_count && to >= 0 &&
          to < network->node_count && weight > 0) {
        network->graph[from][to] = weight;
        printf("Connection modified: %d -> %d (new weight: %d)\n", from, to,
               weight);
      } else {
        printf("Invalid input. No changes made.\n");
      }
      break;

    default:
      printf("Invalid choice. No changes made.\n");
  }
}


bool is_valid_node(network_topology* network, int node) {
    return (node >= 0 && node < network->node_count);
}