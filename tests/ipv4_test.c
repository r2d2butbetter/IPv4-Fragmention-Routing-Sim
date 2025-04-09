/**
 * ipv4_test.c
 * Test program for IPv4 Fragmentation
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include "../include/ipv4.h"
 
 // Function to print IP address in readable format
 void print_ip_address(uint32_t ip) {
     printf("%d.%d.%d.%d", 
         (ip >> 24) & 0xFF,
         (ip >> 16) & 0xFF,
         (ip >> 8) & 0xFF,
         ip & 0xFF);
 }
 
 // Function to display packet information
 void display_packet_info(ipv4_packet* packet) {
     printf("IPv4 Header:\n");
     printf("  Version: %d\n", (packet->header.version_ihl >> 4) & 0x0F);
     printf("  IHL: %d (bytes: %d)\n", packet->header.version_ihl & 0x0F, (packet->header.version_ihl & 0x0F) * 4);
     printf("  Total Length: %d bytes\n", packet->header.total_len);
     printf("  Identification: 0x%04X\n", packet->header.identifier);
     printf("  Flags: 0x%X\n", (packet->header.flags_frag_offset >> 13) & 0x07);
     printf("  Fragment Offset: %d\n", packet->header.flags_frag_offset & 0x1FFF);
     printf("  TTL: %d\n", packet->header.ttl);
     printf("  Protocol: %d\n", packet->header.protocol);
     printf("  Checksum: 0x%04X\n", packet->header.checksum);
     printf("  Source IP: ");
     print_ip_address(packet->header.source_ip);
     printf("\n  Destination IP: ");
     print_ip_address(packet->header.dest_ip);
     printf("\n");
     printf("  Payload Size: %d bytes\n", packet->payload_size);
     
     // Print first few bytes of payload for verification
     printf("  Payload (first 16 bytes): ");
     for (int i = 0; i < 16 && i < packet->payload_size; i++) {
         printf("%02X ", packet->payload[i]);
     }
     printf("\n");
 }
 
 // Function to display fragment information
 void display_fragment_info(ipv4_fragment* fragment) {
     printf("  Total Length: %d bytes\n", fragment->header.total_len);
     printf("  Identification: 0x%04X\n", fragment->header.identifier);
     printf("  Flags: 0x%X\n", (fragment->header.flags_frag_offset >> 13) & 0x07);
     printf("  Fragment Offset: %d\n", fragment->header.flags_frag_offset & 0x1FFF);
     printf("  Data Size: %d bytes\n", fragment->data_size);
     
     // Print first few bytes of data for verification
     printf("  Data (first 16 bytes): ");
     for (int i = 0; i < 16 && i < fragment->data_size; i++) {
         printf("%02X ", fragment->data[i]);
     }
     printf("\n");
 }
 
 int main() {
     // Test parameters
     int source = 0xC0A80101;      // 192.168.1.1
     int destination = 0xC0A80102; // 192.168.1.2
     int payload_size = 1500;
     
     printf("=== IPv4 Functionality Test ===\n\n");
     
     // Create the IPv4 packet
     ipv4_packet packet;
     create_ipv4_packet(&packet, source, destination, payload_size);
     
     printf("=== Original Packet Details ===\n");
     display_packet_info(&packet);
     
     // Test case 1: No fragmentation needed (MTU is large enough)
     int mtu1 = 2000;  // Larger than packet size
     ipv4_fragment* fragments1;
     printf("\n=== Test Case 1: No Fragmentation (MTU = %d) ===\n", mtu1);
     int num_fragments1 = fragment_ipv4_packet(&packet, mtu1, &fragments1);
     
     printf("Number of fragments: %d\n\n", num_fragments1);
     
     for (int i = 0; i < num_fragments1; i++) {
         printf("Fragment %d:\n", i + 1);
         display_fragment_info(&fragments1[i]);
         printf("\n");
     }
     
     // Test case 2: Fragmentation needed (MTU is smaller than packet)
     int mtu2 = 500;  // Smaller than packet size
     ipv4_fragment* fragments2;
     printf("\n=== Test Case 2: With Fragmentation (MTU = %d) ===\n", mtu2);
     int num_fragments2 = fragment_ipv4_packet(&packet, mtu2, &fragments2);
     
     printf("Number of fragments: %d\n\n", num_fragments2);
     
     for (int i = 0; i < num_fragments2; i++) {
         printf("Fragment %d:\n", i + 1);
         display_fragment_info(&fragments2[i]);
         printf("\n");
     }
     
     // Test case 3: Extreme fragmentation with small MTU
     int mtu3 = 100;  // Very small MTU
     ipv4_fragment* fragments3;
     printf("\n=== Test Case 3: Extreme Fragmentation (MTU = %d) ===\n", mtu3);
     int num_fragments3 = fragment_ipv4_packet(&packet, mtu3, &fragments3);
     
     printf("Number of fragments: %d\n\n", num_fragments3);
     
     // Only print the first 5 fragments to avoid flooding the console
     int fragments_to_print = (num_fragments3 > 5) ? 5 : num_fragments3;
     for (int i = 0; i < fragments_to_print; i++) {
         printf("Fragment %d:\n", i + 1);
         display_fragment_info(&fragments3[i]);
         printf("\n");
     }
     
     if (num_fragments3 > 5) {
         printf("... (remaining %d fragments not shown)\n", num_fragments3 - 5);
     }
     
     // Free allocated memory
     if (num_fragments1 > 0) {
         for (int i = 0; i < num_fragments1; i++) {
             free(fragments1[i].data);
         }
         free(fragments1);
     }
     
     if (num_fragments2 > 0) {
         for (int i = 0; i < num_fragments2; i++) {
             free(fragments2[i].data);
         }
         free(fragments2);
     }
     
     if (num_fragments3 > 0) {
         for (int i = 0; i < num_fragments3; i++) {
             free(fragments3[i].data);
         }
         free(fragments3);
     }
     
     // Free packet payload
     free(packet.payload);
     
     printf("\n=== Test completed successfully ===\n");
     return 0;
 }
 
 
 