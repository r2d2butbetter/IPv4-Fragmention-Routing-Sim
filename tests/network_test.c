/**
 * network_test.c
 * Test program for Network Topology functionality
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/network.h"

// Function to display network topology information
void display_network_info(network_topology* network) {
    printf("Network Topology:\n");
    printf("  Number of nodes: %d\n", network->node_count);
    printf("  Adjacency Matrix:\n");
    
    // Print column headers
    printf("    ");
    for (int i = 0; i < network->node_count; i++) {
        printf("%2d ", i);
    }
    printf("\n    ");
    for (int i = 0; i < network->node_count; i++) {
        printf("---");
    }
    printf("\n");
    
    // Print adjacency matrix with row labels
    for (int i = 0; i < network->node_count; i++) {
        printf("%2d | ", i);
        for (int j = 0; j < network->node_count; j++) {
            printf("%2d ", network->graph[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Function to verify a connection exists with the correct weight
int verify_connection(network_topology* network, int from, int to, int expected_weight) {
    int actual_weight = network->graph[from][to];
    if (actual_weight == expected_weight) {
        printf("  ✓ Connection %d->%d has correct weight: %d\n", from, to, expected_weight);
        return 1;
    } else {
        printf("  ✗ Connection %d->%d has weight %d, expected %d\n", 
               from, to, actual_weight, expected_weight);
        return 0;
    }
}

int main() {
    network_topology network;
    int test_passed = 0;
    int total_tests = 0;
    
    printf("=== Network Topology Functionality Test ===\n\n");
    
    // Test 1: Initialize network topology
    printf("=== Test Case 1: Network Initialization ===\n");
    init_network_topology(&network, 5);
    printf("Network initialized with 5 nodes\n");
    display_network_info(&network);
    
    // Verify initialization (all weights should be 0)
    int init_correct = 1;
    for (int i = 0; i < network.node_count; i++) {
        for (int j = 0; j < network.node_count; j++) {
            if (network.graph[i][j] != 0) {
                init_correct = 0;
                printf("  ✗ Initialization failed: Position [%d][%d] = %d, expected 0\n", 
                       i, j, network.graph[i][j]);
            }
        }
    }
    
    if (init_correct) {
        printf("  ✓ Network correctly initialized with all weights set to 0\n");
        test_passed++;
    }
    total_tests++;
    
    // Test 2: Add connections
    printf("\n=== Test Case 2: Adding Connections ===\n");
    add_connection(&network, 0, 1, 5);
    add_connection(&network, 1, 2, 3);
    add_connection(&network, 2, 3, 7);
    add_connection(&network, 3, 4, 2);
    add_connection(&network, 4, 0, 9);
    
    printf("Added 5 connections to the network\n");
    display_network_info(&network);
    
    // Verify connections were added correctly
    int connections_correct = 1;
    connections_correct &= verify_connection(&network, 0, 1, 5);
    connections_correct &= verify_connection(&network, 1, 2, 3);
    connections_correct &= verify_connection(&network, 2, 3, 7);
    connections_correct &= verify_connection(&network, 3, 4, 2);
    connections_correct &= verify_connection(&network, 4, 0, 9);
    
    if (connections_correct) {
        printf("  ✓ All connections added correctly\n");
        test_passed++;
    }
    total_tests++;
    
    // Test 3: Test error handling for invalid nodes
    printf("\n=== Test Case 3: Error Handling for Invalid Nodes ===\n");
    printf("Attempting to add connection from node -1 to node 2:\n");
    add_connection(&network, -1, 2, 10);
    printf("Attempting to add connection from node 1 to node 10 (out of range):\n");
    add_connection(&network, 1, 10, 10);
    
    // Verify that is_valid_node works correctly
    printf("\nTesting is_valid_node function:\n");
    int valid_node_tests = 1;
    valid_node_tests &= (is_valid_node(&network, 0) == true);
    valid_node_tests &= (is_valid_node(&network, 4) == true);
    valid_node_tests &= (is_valid_node(&network, 5) == false);
    valid_node_tests &= (is_valid_node(&network, -1) == false);
    
    if (valid_node_tests) {
        printf("  ✓ is_valid_node function works correctly\n");
        test_passed++;
    } else {
        printf("  ✗ is_valid_node function failed\n");
    }
    total_tests++;
    
    // Test 4: Test predefined topology creation
    printf("\n=== Test Case 4: Create Test Topology ===\n");
    create_test_topology(&network);
    printf("Test topology created\n");
    display_network_info(&network);
    
    // Verify some key connections in the test topology
    int test_topology_correct = 1;
    test_topology_correct &= verify_connection(&network, 0, 1, 2);
    test_topology_correct &= verify_connection(&network, 0, 2, 4);
    test_topology_correct &= verify_connection(&network, 1, 2, 1);
    test_topology_correct &= verify_connection(&network, 3, 5, 1);
    
    if (test_topology_correct) {
        printf("  ✓ Test topology created correctly\n");
        test_passed++;
    } else {
        printf("  ✗ Test topology has incorrect connections\n");
    }
    total_tests++;
    
    // Test 5: Modify network topology
    printf("\n=== Test Case 5: Modify Network Topology (Simulation) ===\n");
    printf("Cannot test modify_network_topology function automatically as it requires user input.\n");
    printf("Please manually test this function separately.\n");
    
    // Print test summary
    printf("\n=== Test Summary ===\n");
    printf("Passed: %d/%d tests\n", test_passed, total_tests);
    
    if (test_passed == total_tests) {
        printf("All tests passed successfully!\n");
    } else {
        printf("Some tests failed. Please check the output above.\n");
    }
    
    return (test_passed == total_tests) ? EXIT_SUCCESS : EXIT_FAILURE;
}
