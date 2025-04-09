/**
 * dijkstra.c
 * Implementation of Dijkstra's algorithm for shortest path calculation
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <stdbool.h>
 #include <limits.h>
 #include "include/dijkstra.h"
 
 int dijkstra(network_topology* network, int source, int destination, int** path) {
     if (!is_valid_node(network, source) || !is_valid_node(network, destination)) {
         printf("Error: Invalid source or destination node.\n");
         return -1;
     }
     
     int n = network->node_count;
     int* dist = (int*)malloc(n * sizeof(int));
     int* prev = (int*)malloc(n * sizeof(int));
     bool* visited = (bool*)malloc(n * sizeof(bool));
     
     if (dist == NULL || prev == NULL || visited == NULL) {
         fprintf(stderr, "Memory allocation failed in Dijkstra's algorithm\n");
         exit(EXIT_FAILURE);
     }
     
     // Initialize arrays
     for (int i = 0; i < n; i++) {
         dist[i] = INT_MAX;
         prev[i] = -1;
         visited[i] = false;
     }
     
     dist[source] = 0;
     
     // Main Dijkstra algorithm
     for (int count = 0; count < n; count++) {
         // Find the unvisited node with minimum distance
         int min_dist = INT_MAX;
         int min_node = -1;
         
         for (int v = 0; v < n; v++) {
             if (!visited[v] && dist[v] < min_dist) {
                 min_dist = dist[v];
                 min_node = v;
             }
         }
         
         // If no node found or we reached the destination, break
         if (min_node == -1 || min_node == destination) break;
         
         // Mark the node as visited
         visited[min_node] = true;
         
         // Update distances to adjacent nodes
         for (int v = 0; v < n; v++) {
             // Check if there's a connection and if we can improve the path
             if (network->graph[min_node][v] > 0 && 
                 !visited[v] && 
                 dist[min_node] != INT_MAX && 
                 dist[min_node] + network->graph[min_node][v] < dist[v]) {
                 
                 dist[v] = dist[min_node] + network->graph[min_node][v];
                 prev[v] = min_node;
             }
         }
     }
     
     // Check if destination is reachable
     if (dist[destination] == INT_MAX) {
         printf("No path exists from node %d to node %d.\n", source, destination);
         free(dist);
         free(prev);
         free(visited);
         return -1;
     }
     
     // Reconstruct the path
     // First, count the number of nodes in the path
     int count = 1; // Start with 1 for the destination node
     int current = destination;
     
     while (current != source) {
         current = prev[current];
         count++;
     }
     
     // Allocate memory for the path
     *path = (int*)malloc(count * sizeof(int));
     if (*path == NULL) {
         fprintf(stderr, "Memory allocation failed for path\n");
         exit(EXIT_FAILURE);
     }
     
     // Fill the path array in reverse (from destination to source)
     current = destination;
     int index = count - 1;
     
     (*path)[index--] = current;
     
     while (current != source) {
         current = prev[current];
         (*path)[index--] = current;
     }
     
     // Free temporary arrays
     free(dist);
     free(prev);
     free(visited);
     
     return count;
 }