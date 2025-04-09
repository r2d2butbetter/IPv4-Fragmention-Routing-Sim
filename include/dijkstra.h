/**
 * dijkstra.h
 * Dijkstra's algorithm for shortest path calculation
 */

 #ifndef DIJKSTRA_H
 #define DIJKSTRA_H
 
 #include "network.h"
 
 // Find the shortest path using Dijkstra's algorithm
 // Returns the path length (-1 if no path exists)
 // Sets path to an array of nodes that form the path (caller must free)
 int dijkstra(network_topology* network, int source, int destination, int** path);
 
 #endif /* DIJKSTRA_H */