#include "include/ui.h"

#include <stdio.h>
#include <stdlib.h>

void display_welcome_banner() {
  printf("\n");
  printf("===============================================\n");
  printf("  IPv4 Fragmentation with Dijkstra Routing\n");
  printf("===============================================\n");
  printf("This program simulates IPv4 packet fragmentation\n");
  printf("and routes each fragment using the shortest path\n");
  printf("calculated with Dijkstra's algorithm.\n");
  printf("-----------------------------------------------\n\n");
}

void get_network_topology(network_topology* network) {
  int ch;

  printf("Choose network topology:\n");
  printf("1. Use predefined test topology\n");
  printf("2. Create custom topology\n");
  printf("Enter choice (1-2): ");
  scanf("%d", &ch);

  switch (ch) {
    case 1:
      create_test_topology(network);
      break;

    case 2:
      create_custom_topology(network);
      break;

    default:
      printf("Invalid choice");
      break;
  }
}

void display_network_topology(network_topology* network) {
  printf("\n=== Network Topology ===\n");
  printf("Number of nodes: %d", network->node_count);

  printf("\nAdjacency Matrix: \n");

  printf("    ");
  for (int i = 0; i < network->node_count; i++) {
    printf("%4d", i);
  }
  printf("\n");

  printf("    ");
  for (int i = 0; i < network->node_count; i++) {
    printf("----");
  }
  printf("\n");

  for (int i = 0; i < network->node_count; i++) {
    printf("%2d |", i);
    for (int j = 0; j < network->node_count; j++) {
      printf("%4d", network->graph[i][j]);
    }
    printf("\n");
  }
  printf("\n");

  // Display the connections in a more readable format
  printf("Connections:\n");
  int connections = 0;
  for (int i = 0; i < network->node_count; i++) {
    for (int j = 0; j < network->node_count; j++) {
      if (network->graph[i][j] > 0) {
        printf("  Node %d -> Node %d (weight: %d)\n", i, j,
               network->graph[i][j]);
        connections++;
      }
    }
  }

  if (connections == 0) {
    printf("  No connections defined.\n");
  }

  printf("\n");
}

void get_user_inputs(network_topology* network, int* source, int* destination, int* mtu, int* payload_size)
{
    // Get source node
    do {
        printf("Enter source node (0-%d): ", network->node_count - 1);
        scanf("%d", source);
    } while (!is_valid_node(network, *source));
    
    // Get destination node
    do {
        printf("Enter destination node (0-%d): ", network->node_count - 1);
        scanf("%d", destination);
    } while (!is_valid_node(network, *destination) || *source == *destination);
    
    // Get MTU
    do {
        printf("Enter MTU (must be at least %d): ", IPV4_HEADER_SIZE + 8);
        scanf("%d", mtu);
    } while (*mtu < IPV4_HEADER_SIZE + 8);
    
    // Get payload size
    do {
        printf("Enter payload size (1-%d): ", MAX_PAYLOAD_SIZE);
        scanf("%d", payload_size);
    } while (*payload_size < 1 || *payload_size > MAX_PAYLOAD_SIZE);
}

void display_packet_info(ipv4_packet* packet)
{
    printf("IPv4 Header:\n");
    printf("  Version/IHL: 0x%02X\n", packet->header.version_ihl);
    printf("  Total Length: %d bytes\n", packet->header.total_len);
    printf("  Identification: 0x%04X\n", packet->header.identifier);
    printf("  Flags/Fragment Offset: 0x%04X\n", packet->header.flags_frag_offset);
    printf("  TTL: %d\n", packet->header.ttl);
    printf("  Protocol: %d\n", packet->header.protocol);
    printf("  Checksum: 0x%04X\n", packet->header.checksum);
    printf("  Source Address: %d\n", packet->header.source_ip);
    printf("  Destination Address: %d\n", packet->header.dest_ip);
    printf("Payload Size: %d bytes\n", packet->payload_size);
}

void display_fragment_info(ipv4_fragment* fragment, int fragment_num)
{
    /* Avoid unused parameter warning */
    (void)fragment_num;
    printf("  Header:\n");
    printf("    Total Length: %d bytes\n", fragment->header.total_len);
    printf("    Identification: 0x%04X\n", fragment->header.identifier);
    
    uint16_t flags = (fragment->header.flags_frag_offset & 0xE000) >> 13;
    uint16_t offset = fragment->header.flags_frag_offset & 0x1FFF;
    
    printf("    Flags: 0x%X (", flags);
    if (flags & 0x4) printf("Don't Fragment, ");
    if (flags & 0x2) printf("More Fragments");
    else printf("Last Fragment");
    printf(")\n");
    
    printf("    Fragment Offset: %d (in 8-byte units)\n", offset);
    printf("    TTL: %d\n", fragment->header.ttl);
    printf("    Data Size: %d bytes\n", fragment->data_size);
}



void display_route_path(ipv4_fragment* fragment)
{
    if (fragment->path_length <= 0 || fragment->path == NULL) {
        printf("  No route available for this fragment.\n");
        return;
    }
    
    printf("  Routing Path (%d hops): ", fragment->path_length - 1);
    for (int i = 0; i < fragment->path_length; i++) {
        printf("%d", fragment->path[i]);
        if (i < fragment->path_length - 1) {
            printf(" -> ");
        }
    }
    printf("\n");
}
