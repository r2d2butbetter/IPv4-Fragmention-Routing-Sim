#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/dijkstra.h"
#include "../include/ipv4.h"
#include "../include/network.h"
#include "../include/ui.h"

int main() {
  network_topology network;
  int source, dest, mtu, payload_size;

  display_welcome_banner();
  get_network_topology(&network);
  display_network_topology(&network);
  get_user_inputs(&network, &source, &dest, &mtu, &payload_size);

  ipv4_packet packet;
  create_ipv4_packet(&packet, source, dest, payload_size);

  printf("===Original Packet Detsails====");
  display_packet_info(&packet);

  ipv4_fragment* fragments;
  int num_frag = fragment_ipv4_packet(&packet, mtu, &fragments);

  printf("\n=== Fragmentation Results ===\n");
  printf("Number of fragments: %d\n\n", num_frag);

  for (int i = 0; i < num_frag; i++) {
    printf("Fragment: %d", i + 1);

    fragments[i].path_length =
        dijkstra(&network, source, dest, &(fragments[i].path));

    display_fragment_info(&fragments[i], num_frag);

    display_route_path(&fragments[i]);

    printf("\n");

    // Optional: Simulate topology change for dynamic routing
    printf(
        "Do you want to change the network topology before sending the next "
        "fragment? (y/n): ");

    char response;
    scanf(" %c", &response);
    if (response == 'y' || response == 'Y') {
      modify_network_topology(&network);
      display_network_topology(&network);
    }
  }
  for (int i = 0; i < num_frag; i++) {
    free(fragments[i].path);
  }
  free(fragments);

  return 0;
}
