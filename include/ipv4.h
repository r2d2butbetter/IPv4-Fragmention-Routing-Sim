#ifndef IPV4_FRAG_H
#define IPV4_FRAG_H

#include <stdint.h>

#define IPV4_HEADER_SIZE 20  // IPv4 header size without options
#define MAX_IPV4_PACKET_SIZE 65535  // Max packet size
#define MAX_PAYLOAD_SIZE (MAX_IPV4_PACKET_SIZE - IPV4_HEADER_SIZE)

typedef struct ipv4_header
{
    //1st row from notes
    uint8_t version_ihl; //4 bit version, 4 header length
    uint8_t tos; //type of servive
    uint16_t total_len; //Datagram Length
    //2nd row
    uint16_t identifier; //16 bit Identifier
    uint16_t flags_frag_offset; //3 bit flags, fragmentation offset(13bits)
    //3rd row
    uint8_t ttl; //8 bit time to live
    uint8_t protocol; //8 bit upper layer protocol
    uint16_t checksum; //Header Checksuim

    //4th row
    uint32_t source_ip;
    uint32_t dest_ip;
} ipv4_header;

// IPv4 packet structure
typedef struct ipv4_packet{
    ipv4_header header;
    uint8_t*   payload;
    uint16_t   payload_size;
} ipv4_packet;

// IPv4 fragment structure
typedef struct ipv4_fragment {
    ipv4_header header;
    uint8_t*   data;
    uint16_t   data_size;
    
    // Routing information
    int*       path;      // Array of nodes that form the path from source to destination
    int        path_length; // Number of nodes in the path
} ipv4_fragment;

// Create a new IPv4 packet
void create_ipv4_packet(ipv4_packet* packet, int source, int destination, int payload_size);

// Fragment an IPv4 packet based on MTU
int fragment_ipv4_packet(ipv4_packet* packet, int mtu, ipv4_fragment** fragments);

// Helper function to calculate IPv4 header checksum
uint16_t calculate_checksum(ipv4_header* header);

#endif