#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/ipv4.h"

static uint16_t packet_id = 1000;

void create_ipv4_packet(ipv4_packet* packet, int source, int destination, int payload_size)
{
    if (payload_size <=0 || payload_size> MAX_PAYLOAD_SIZE)
    {
        printf("Packet Size is impossible, adjusting to normal");
        payload_size = MAX_PAYLOAD_SIZE;
    }


    packet->header.version_ihl= 0x45;
    packet->header.tos = 0;
    packet->header.total_len= payload_size + IPV4_HEADER_SIZE;
    packet->header.identifier= packet_id++;
    packet->header.flags_frag_offset = 0x4000;
    packet->header.ttl=64;
    packet->header.protocol= 17; // UDP

    packet->header.source_ip=source;
    packet->header.dest_ip=destination;

    packet->payload = (uint8_t*)(malloc(payload_size));
    //Later: Add error incase malloc fails

    for (int i = 0; i < payload_size; i++) {
        packet->payload[i] = i % 256;
    }

    packet->payload_size= payload_size;

    packet->header.checksum= 0;
    packet->header.checksum= calculate_checksum(&packet->header);
}

int fragment_ipv4_packet(ipv4_packet* packet, int mtu, ipv4_fragment** fragments)
{
    if (packet->header.total_len <= mtu) //no fragmentation
    {
        *fragments = (ipv4_fragment*)malloc(sizeof(ipv4_fragment));
        if (*fragments == NULL) {
            printf("Memory allocation failed\n");
            return 0;
        }
    
        (*fragments)[0].header = packet->header;
        (*fragments)[0].data_size = packet->payload_size;
        (*fragments)[0].data = (uint8_t*)malloc(packet->payload_size);
        if ((*fragments)[0].data == NULL) {
            printf("Memory allocation failed\n");
            free(*fragments);
            return 0;
        }

        memcpy((*fragments)[0].data, packet->payload, packet->payload_size);

        (*fragments)[0].path_length = 0;
        (*fragments)[0].path = NULL;

        return 1; //return 1 fragment(original one)
    }
    else //there is fragmentation
    {
        int max_per_fragment = (mtu - IPV4_HEADER_SIZE) & ~0x7;//roundoff to 8

        int num_fragments = (packet->payload_size + max_per_fragment - 1) / max_per_fragment;

        *fragments = (ipv4_fragment*)malloc(sizeof(ipv4_fragment) * num_fragments);
        if (*fragments == NULL) {
            printf("Memory allocation failed\n");
            return 0;
        }

        int remaining_data = packet->payload_size;
        int offset = 0;

        for (int i = 0; i < num_fragments; i++)
        {
            int fragment_size = (remaining_data < max_per_fragment) ? remaining_data : max_per_fragment;

            (*fragments)[i].header = packet->header;
            (*fragments)[i].header.total_len = IPV4_HEADER_SIZE + fragment_size;

            uint16_t frag_offset = offset / 8; //units of 8

            if (i < num_fragments - 1) //there are more left
            {
                (*fragments)[i].header.flags_frag_offset = 0x2000 | frag_offset;  //More Fragments bit set to 1
            }
            else //this is the last fragment
            {
                (*fragments)[i].header.flags_frag_offset = frag_offset; //No More Fragments flag for last fragment
            }
            
            //allocate and copy fragment data
            (*fragments)[i].data_size = fragment_size;
            (*fragments)[i].data = (uint8_t*)malloc(fragment_size);
            if ((*fragments)[i].data == NULL) {
                printf("Memory allocation failed\n");
                // Free previously allocated fragments
                for (int j = 0; j < i; j++) {
                    free((*fragments)[j].data);
                }
                free(*fragments);
                return 0;
            }

            memcpy((*fragments)[i].data, packet->payload + offset, fragment_size);

            //path info
            (*fragments)[i].path_length = 0;
            (*fragments)[i].path = NULL;

            //recalc checksum for this fragment
            (*fragments)[i].header.checksum = 0;
            (*fragments)[i].header.checksum = calculate_checksum(&((*fragments)[i].header));
            
            // Update for next fragment
            offset += fragment_size;
            remaining_data -= fragment_size;
        }
        
        return num_fragments;
    }
}

uint16_t calculate_checksum(ipv4_header* header)
{
    /* Avoid unused parameter warning */
    (void)header;
    return 0xABCD;
}

