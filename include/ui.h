/**
 * ui.h
 * User interface functions
 */

 #ifndef UI_H
 #define UI_H
 
 #include "network.h"
 #include "ipv4.h"
 
 // Display the welcome banner and program information
 void display_welcome_banner();
 
 // Get network topology from user (choose predefined or custom)
 void get_network_topology(network_topology* network);
 
 // Display the network topology
 void display_network_topology(network_topology* network);
 
 // Get user inputs for packet parameters
 void get_user_inputs(network_topology* network, int* source, int* destination, int* mtu, int* payload_size);
 
 // Display packet information
 void display_packet_info(ipv4_packet* packet);
 
 // Display fragment information
 void display_fragment_info(ipv4_fragment* fragment, int fragment_num);
 
 // Display the route path for a fragment
 void display_route_path(ipv4_fragment* fragment);
 
 #endif /* UI_H */